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
Requesting and releasing memory is complicated and this obstacle has **suffocated software innovation** across corporation and government ventures for years. Utilizing **arenas** is the **silver bullet** aimed against the **vain complexity** to free the people to architect a greater digital ecosystem.

Programmers today rely on **slow memory-garbage-collectors** and/or manually manage memory with `malloc`/`free`/`new`/`delete`. These functions have **exhausting levels of complexity** under the hood (managing and updating lists of pointers to free blocks and used blocks of memory). Their misuse under mental loads add more **vulnerabilities**, **performance considerations**. Furthermore, if utilizing the common C standard library (GNU glibc), one adds **4614 lines** of program logic to the project. Programmers need to keep track of every request and release which removes mental clarity and product focus. It is **a grievous** negative factor against program **simplicity & organization** leading to **heavy, barely-maintainable** code bases. 

Regarding the silver bullet. The arena.h/arena.c contains less than **418 lines** of program logic for memory management. Its internal protocols and functions manage memory by indices and offsets rather than pointers. No complex linked lists of free blocks vs allocated blocks. Pointers are automatically updated when arenas grow/remap themselves (with **zero compute** necessary). The programmer's code will use the arena_start(block) + the stored offset to find, read, and write data to the correct locations in memory. Each arena can map at most (4GB - 4KB). Many arenas can be mapped until hardware runs out. One can request/release any arena(s) with arena_map(block)/arena_unmap(block) while any remaining will be cleared at program exit. The capability to lock arenas into memory (no swapping to disk allowed) is provided for cryptographic applications with arena_special_map(amount) and arena_special_unmap(block).
