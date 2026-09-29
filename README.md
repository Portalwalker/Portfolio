# C Code Portfolio

A collection of advanced C programming solutions: custom memory allocation, syscall handling, 68 years more support for 2038 problem on 32 bit systems.
No C Standard Library dependencies.

## Repository Structure

* **code** — Source Code and Header pairs. One source code and header per directory allowed (e.g., `arena.h` / `arena.c`).
* **compile** — Contains 7 script files that completely replace the functionality of `make`.
* **compile/dep** — Holds all symbolic links to source code headers, as well as the ultimate static library used to link to the final binary.
* **dep** — Found inside nearly every source code directory to facilitate the compilation process.

### How to get up and running.
```bash
source ./compile/globals
compile && run && clean
```

### Dependencies
1. Bash
2. Compiler: gcc (gnu99), ar
3. Common Tools: find, grep, realpath, sha256sum, sort, tr, uniq
