# The Unlink Exploit
When a chunk is freed, it's placed into a linked list with other free chunks
that are used by the allocator. However, if there are two consecutive free
chunks, the second chunk is consolidated with the memory of the first chunk and
then *removed from its free list* so that the new, larger chunk can be inserted
into the unsorted bin, a fast cache list of recently freed chunks. See this code
sample from a vulnerable libc, where `p` is the chunk being freed:

    /* consolidate backward */
    if (!prev_inuse(p)) {
      prevsize = p->prev_size;
      size += prevsize;
      p = chunk_at_offset(p, -((long) prevsize));
      unlink(p, bck, fwd);
    }

    if (nextchunk != av->top) {
    /* get and clear inuse bit */
    nextinuse = inuse_bit_at_offset(nextchunk, nextsize);

        /* consolidate forward */
        if (!nextinuse) {
            unlink(nextchunk, bck, fwd);
            size += nextsize;
        } else
            clear_inuse_bit_at_offset(nextchunk, 0);
    ...

There's a lot going on here. The macro `prev_inuse` just checks if that bit is
set (see [layout](layout)), and if it's not, the two chunks will be joined
together (the one that is being freed and the one before it). This is done by
moving the target pointer `p` back by the size of the previous chunk (the second
last line) and then executing the `unlink` macro on that chunk (`fwd` and `bck`
are temporary variables that `unlink` uses; they are not initialised). What does
`unlink` do?

    #define unlink(P, BK, FD) {
        FD = P->fd;
        BK = P->bk;
        FD->bk = BK;
        BK->fd = FD;
    }

Woah! That's overwriting memory, which makes sense since it's removing an
element from a doubly-linked list. But what if those pointers were somehow
malicious? Then `unlink` would write one pointer to another (plus a few bytes on
side because the `fd` and `bk` are the third and fourth [structure](layout)
members, respectively). Suppose a program did the following:

    char * a = malloc(128); // large enough to not be put in the fastbin
    malloc(64); // so that a is not the top
    fgets(a, 256, stdin); // read in double the size (buffer overflow)
    ...
    free(a);

128 bytes of memory can be written past the end of `a`, either into the next
chunk or into the top chunk or wilderness, a special chunk that sotres the
remaining unused memory on the heap. These 128 bytes could be used to fabricate
a fake free chunk (if the next chunk is not the top), setting fake values for
the forward and back pointers. The goal will be to reach the "consolidate
forward" block. After checking the prev-inuse bit of the chunk two
forward of the overflowed chunk (the 0 at the end of the last block in that
diagram), the first fake chunk will be unlinked, with the malicious pointers
being passed in to `unlink`.

Alternatively, this exploit could also affect a write-after-free[^waf]. Suppose
two chunks were allocated, then one was freed and had its `fwd` and `bck`
pointers overwritten. Then, if the other one was freed then the consolidation of
the chunks would lead to `unlink()`  being called on the chunk's modified
pointers and could be exploited in the same way. These techniques both lead to a
semi-arbitrary write, as shown in the [examples](examples).

[^waf]:
    Some other texts use the phrase 'use-after-free' to refer to both reads and
    writes after a chunk is freed.
