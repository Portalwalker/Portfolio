# C Code Portfolio

A collection of advanced C programming solutions: custom memory allocation, syscall handling, 68 years more support for 2038 problem on 32 bit systems.
No C Standard Library dependencies.

## Repository Structure

* `code` - Source Code and Header pairs. One source code and header per directory allowed. (e.g. dir: arena.h/arena.c)
* `compile` - 7 script files that completely replace 'make'.
* `compile/dep` - Holds all symbolic links to source code headers. Holds the ultimate static library to link to the final binary.
* `dep` - Shows up in almost every source code directory to facilitate compilation.
* `./` - The top directory containing: code, compile, dep (link), main.c, main.run, LICENSE.md, & README.md

### How to get up and running.
```bash
source ./compile/globals
compile && run && clean
```
