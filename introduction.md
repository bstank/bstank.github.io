# Introduction

This site is an explanation of *heap exploits*, in particular the heap
allocation algorithims in GNU libcs from the 21st century on Linux, x86\_64.  If
you don't know any of these terms, read ahead! This page will get you up to
speed on memory allocation If you do, feel free to peruse
the [contents](contents) or go to the next section.

## Memory

Doing almost anything with a computer requires *memory* (also known as RAM),
which holds the things that the computer is working on at the time, such as the
sum of a column in a spreadsheet, the contents of a document before you save it,
the song that's currently playing, etcetera. But applications use different
amounts of memory, and often don't know how much memory they are going to need,
it needs to be *allocated* to them while they're running.

## Allocation

On Linux, userspace applications can request memory from the kernel using the
`brk(2)`, `sbrk(2)`, or `mmap(2)` system calls. However, these calls can be
somewhat slow (but still very fast on a human scale!), so requesting a lot of
small pieces of memory is not advisable, and neither is asking for and returning
memory constantly. Also, using these calls can make your program non-portable,
as the calls and their optimal usage can vary from platform to platform.

## The Heap

The *heap*[^1] solves these issues. In
[C](https://en.wikipedia.org/wiki/C_\(programming_language\)/), memory is
allocated by the library[^2] functions `malloc(3)` and `realloc(3)`, and
returned by `free(3)`. Note that memory that is freed is not necessarily
returned to the system, but can be saved for future use by the application. When
a program calls malloc for the first time, one of the aforementioned system
calls is used to a get a large chunk of memory, which can then be parceled out
to the application in chunks.

[^1] This is distinct from the data structure. A memory heap could be
implemented with a heap, but doesn't have to be.
[^2] Library functions are standard pieces of code that are available widely.

## What Could Go Wrong?

Heap systems need to store information about the chunks of memory they have
given out and gotten back to the user (such as the size of the chunk). This
information is often stored *inline* (before, after, or at the start of the
block allocated from the system, in general) with the chunks of memory that are
given to the user. If this information is corrupted[^2], then many different
things can go wrong; in the worst case, a malicious user can inject their own
code to be executed.

## Not So Fast..

There are many different ways to misuse heap implementations that can lead to
issues. Freeing memory multiple times, reading or writing past the boundaries of
a chunk, and reading or writing a chunk after it's been freed are the most
common ones. However, these can be difficult to exploit, and the exploitation is
very dependent on the heap algorithm being used. This website covers some common
exploits for the [GNU](https://www.gnu.org/) C library (glibc), which is very
commonly used.
