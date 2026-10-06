# Portfolio

A collection of advanced Pure C programming solutions including:
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

Just `#include "dep/onHim.h"`. in 'main.c'<br>
and `void _start()` <br>
$~~~~~~~$ `{` <br>
$~~~~~~~$ `    u32 written = write(STDOUT, "Hello World!\n", 13);` <br>
$~~~~~~~$ `    exit(0);` <br>
$~~~~~~~$ `}` <br>
and `compile && run` <br>


## Repository Structure
* code — One same-prefix source code/header pair per directory allowed. Many single headers allowed.
* compile — Contains 7 script files that replace the functionality of `make`.
* compile/dep — A directory containing links to all source code headers.

### How to get up and running.
```bash
git clone https://github.com/Portalwalker/Portfolio.git
cd Portfolio; source ./compile/globals
compile && run
clean # optional
```

### Dependencies
* Shell $~~~~~~~~~~~~$ bash \\
* Compiler $~~~~~~~~~$ gcc \\
* Library Builder $~~~~$ ar \\
* Version Control $~~~~$ git \\

#### Personal Notes on Memory Management
Programmers either rely on a slow memory-garbage-collector or manage their own memory with `malloc`/`free` or `new`/`delete`. These functions have an exhausting level of complexity under the hood (managing and updating lists of pointers to free blocks and used blocks of memory). Yet another surface for bug hunting, perfomance consideration, and 5000+ lines of code included. Programmers need to keep track of every `malloc` and `free` and this takes away mental clarity while building product. This is a grievous dragging factor in program accuracy and simplicity and is barely manageable when trying to expand a large code base (e.g. a weapon system, an embedded system network mesh, or even a startup product). Many low-level programming software projects suffer from this. 

The arena.h/arena.c contains less than 600 lines code for memory management. Its internal protocols and functions manage memory with arenas, offsets, and indexing. Memory mappings grow by themselves. The programmer can control memory locking for cryptographic applications. No complex linked lists of free blocks vs allocated blocks. All pointers are automatically updated when arenas grow/remap themselves (with zero compute necessary). This is because the programmer's code will rely on the start of arena + the stored offset of whatever data is being processed in that arena. A programmer can map more and more memory into various memory arenas until the the hardware runs out. One can release the arena(s) in use at any time and any remaining will be cleared at program exit.

Requesting and releasing memory has been over complicated for decades and this has lead to the invisible suffocation of software innovation across many corporations and government ventures. Utilizing memory arenas takes away the logical load of requests/releases/pointer-updates off the mind of the software architect/programmer and allows increased focus on the product instead of technical debt.
