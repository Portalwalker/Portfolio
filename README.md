# C Code Portfolio

A collection of advanced C programming solutions: custom memory allocation, syscall handling, 68 years more support for 2038 problem on 32 bit systems.
No C Standard Library dependencies.

## Repository Structure

* **code** — One source code/header pair per new directory allowed. Many non-source-code headers allowed.
* **compile** — Contains 7 script files that replace the functionality of `make`.
* **compile/dep** — Holds all symbolic links to source code headers and the static library for the final binary.
* **dep** — Found inside nearly every source code directory to facilitate the compilation process.

### How to get up and running.
```bash
git clone https://github.com/Portalwalker/CIA.git; source ./compile/globals
compile && run && clean
```

### Dependencies
* Bash
* Compiler => gcc, ar
* Common => find, grep, realpath, sha256sum, sort, tr, uniq
* git
