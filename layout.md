A chunk that is allocated on the heap stores certain metadata about itself and
the previous chunk. The `malloc_chunk` structure defines this, in this case from
glibc 2.34[^1].

[^1]:
    The `nextsize` pointers are not in all versions, but they are fairly
    inconsequential for our purposes.


    struct malloc_chunk {
      INTERNAL_SIZE_T prev_size; /* Size of previous chunk (if free). */
      INTERNAL_SIZE_T size;      /* Size in bytes, including overhead. */
      struct malloc_chunk* fd;   /* double links -- used only if free. */
      struct malloc_chunk* bk;
      struct malloc_chunk* fd_nextsize; /* for large blocks in newer libcs */
      struct malloc_chunk* bk_nextsize;
    };

This structure is somewhat confusing. The actual memory returned to the user
starts in the same place as the `fd` pointer, as the `fd` and `bk` pointers are
only used when the chunk is free. Because the size of a chunk is always rounded
up to at least an eight-byte (double word) boundary, the last three bits can be
used to store flags, known as `A`[^2], `M`[^3], and `P`. Only `P` (short for
prev-inuse) is relevant for now. It stores whether the previous chunk is
allocated (in use).

[^2]:
    Whether the chunk is in the main arena.

[^3]:
    Whether the chunk is mmapped.


    start of chunk  -> +----------------------------------------+
                       | Size of previous chunk if it is free   |
                       +----------------------------------------+
                       | Size of this chunk               |A|M|P|
    start of memory -> +----------------------------------------+
                       | Application data...                    |
                       .                                        .
                       .                                        .
    start of next   -> +----------------------------------------+

Free chunks[^4] are stored in doubly-linked lists, like this:

    start of chunk  -> +----------------------------------------+
                       | Size of previous chunk if it is free   |
                       +----------------------------------------+
                       | Size of this chunk               |A|M|P|
    start of memory -> +----------------------------------------+
                       | Forwards pointer (next chunk)          |
                       +----------------------------------------+
                       | Backwards pointer (previous chunk)     |
                       +----------------------------------------+
                       | Unused (can contain stale data)        |
                       .                                        .
                       .                                        .
    start of next   -> +----------------------------------------+

[^4]:
    In general; the [tcache](tcache) and the fastbins break these rules.

