# Portfolio
A collection of advanced Pure C programming solutions

## Features
* No C Standard Library dependencies
* No Makefile dependency mess
* Robust memory allocation
* A 68 year fix for the inevitable post-2038 problem on 32-bit systems (A.K.A. Gen-Z's Y2K)
* Organized Linux System Calls supporting:

  - `x86_64`$~~~~~~~~~~~$(x86 64-bit)
  - `i386`$~~~~~~~~~~~~~~~$(x86 32-bit)
  - `ARM64`$~~~~~~~~~~~~~$(arm 64-bit)
  - `ARM EABI`$~~~~~~~$(arm 32-bit)
  - `ARM OABI`$~~~~~~~$(arm 32-bit) **old/rare*
  - `MIPS N64`$~~~~~~~$(mips 64-bit)
  - `MIPS N32`$~~~~~~~$(mips 32-bit addrs + 64-bit regs)
  - `MIPS O32`$~~~~~~~$(mips 32-bit)

## Simple How-To-Use
Just `#include "dep/onHim.h"`. in `main.c` <br>
and `void _start()` <br>
$~~~~~~~$ `{` <br>
$~~~~~~~$ `    u32 written = write(STDOUT, "Hello World!\n", 13);` <br>
$~~~~~~~$ `    exit(0);` <br>
$~~~~~~~$ `}` <br>
and `compile && run` <br>

### Setup and Demo
```bash
git clone https://github.com/Portalwalker/Portfolio.git
cd Portfolio; source ./compile/globals
compile && run
clean # optional
```

## Repository Structure
* Portfolio/. — Top directory. You can compile/run from anywhere after `source .../Portfolio/compile/globals`
* Portfolio/code — One same-prefix source code/header pair per directory allowed. Many single headers allowed.
* Portfolio/compile — Contains 7 script files that replace the functionality of `make`.
* Portfolio/compile/dep — A directory containing links to all source code headers.

### Dependencies
* Shell $~~~~~~~~~~~~$ bash \\
* Compiler $~~~~~~~~~$ gcc \\
* Library Builder $~~~~$ ar \\
* Version Control $~~~~$ git \\

#### Personal Notes on Memory Management
Requesting and releasing memory is complicated and this obstacle has suffocated software innovation across corporation and government ventures for years. Utilizing memory arenas is the silver bullet against this load of complexity and frees the mind of technologically adept architects to build stronger and faster products.

Programmers rely on a slow memory-garbage-collectors or manage memory with `malloc`/`free`/`new`/`delete`. These functions have exhausting levels of complexity under the hood (managing and updating lists of pointers to free blocks and used blocks of memory). They add more vulnerabilities, performance considerations, and approx. 5000+ lines of code included (if using the C standard library). Programmers need to keep track of every request and release which removes mental clarity and product focus. It is **a grievous** negative factor in program accuracy and simplicity leading to heavy, barely-maintainable code bases. 

Regarding the silver bullet. The arena.h/arena.c contains less than 600 lines code for memory management. Its internal protocols and functions manage memory by indices and offsets rather than pointers. No complex linked lists of free blocks vs allocated blocks. All pointers are automatically updated when arenas grow/remap themselves (with **zero compute** necessary). This is because the programmer's code will rely on the start of arena + the stored offset of whatever data is being processed in that arena. A programmer can map more and more memory into various memory arenas until the the hardware runs out. One can release the arena(s) in use at any time and any remaining will be cleared at program exit. The capability to lock arenas into memory (no swapping to disk allowed) is provided for cryptographic applications.
