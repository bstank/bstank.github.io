# History

The GNU libc used, for a fair while, a heap called *dlmalloc*, written by Doug
Lea. Exploits for this malloc interface were well documented in Phrack,
especially by the two articles *Vudo malloc tricks* and  *Once upon a free()*,
both in the 57th issue. The main idea at the time was to exploit a macro used in
the freeing of chunks called `unlink()`, which allowed an attacker to write an
arbitrary address to another arbitrary address[^1].

[^1]:
    This was not entirely arbitrary if the memory was not read-writable.

Then, in glibc v2, the malloc functions were adapted by Wolfram Gloger for
thread support (the pt in ptmalloc refers to POSIX threads). In 2004,
with the glibc 2.3.4 release, a number of security checks were added to make it
harder to exploit the heap. In particular, the `unlink()` macro added a simple,
effectively unsurmountable security check.  Despite this, there were still
vulnerabilites, explained in a 2005 text called *The Malloc Maleficarum*, which
was somewhat incomplete, and was extened in 2009 by the article *Malloc
Des-Maleficarum*. Other techniques have been published over time, as patches to
glibc remove others.
