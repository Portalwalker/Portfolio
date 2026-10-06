# C Code Portfolio

A collection of advanced C programming solutions: custom memory allocation, syscall handling, 68 years more support for 2038 problem on 32 bit systems.
No C Standard Library dependencies.

## Repository Structure

#### code — One same-prefix source code/header pair per directory allowed. Many single headers allowed.
#### compile — Contains 7 script files that replace the functionality of `make`.

## Important Code
Programmers either rely on a slow memory-garbage-collector or manage their own memory with malloc/free or new/delete. These functions have an exhausting level of complexity under the hood (managing and updating lists of pointers to free blocks and used blocks of memory. Yet another surface for bug hunting, perfomance consideration, and 5000+ lines of code included. Programmers need to keep track of every 'malloc' and 'free' and his takes up part of his/her mind while working on a project. This is a grievous dragging factor in program accuracy and is barely manageable when trying to expand a large code base (e.g. a weapon system, an embedded system network mesh, or a startup product). Most low-level programming software projects suffer from this. 

The arena.h/arena.c contains less than 600 lines code for memory management. Its internal protocols and functions manage memory with arenas, offsets, and indexing. Memory mappings grow by themselves. The programmer can control memory locking for cryptographic applications. No complex linked lists of free blocks vs allocated blocks. All pointers are automatically updated when arenas grow/remap themselves (with zero compute necessary). This is because the programmer's code will rely on the start of arena + the stored offset of whatever data is being processed in that arena. A programmer can map more and more memory into various memory arenas until the the hardware runs out. 

Requesting and releasing memory has been over complicated for decades and this has lead to the invisible suffocation of software innovation across many corporations and government ventures. Utilizing memory arenas takes away the logical load of requests/releases/pointer-updates off the mind of the software architect/programmer and allows increased focus on the product instead of technical debt.

### How to get up and running.
```bash
git clone https://github.com/Portalwalker/SilverMemory.git; source ./compile/globals
compile && run && clean
```

### Dependencies
* Bash
* Compiler => gcc, ar
* Common => find, grep, realpath, sha256sum, sort, tr, uniq
* git
