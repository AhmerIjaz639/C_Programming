# Day 01 — C Fundamentals for Operating Systems

##  Objective

Build the C foundation required for systems and OS programming, with focus on how C code becomes a Linux executable.

##  Core Knowledge

### C Fundamentals

* Variables & data types: `int`, `char`, `float`
* `if/else`, loops, arrays
* Functions and return values
* `sizeof()` → returns size in **bytes**
* `argc` / `argv` → command-line arguments

Example:

```c
int main(int argc, char *argv[])
```

##  GCC & Compilation

Basic:

```bash
gcc -Wall -Wextra main.c -o main
./main
```

Object file:

```bash
gcc -c main.c -o main.o
```

Build flow:

```text
.c → Compiler → .o → Linker → Executable
```

`-Wall -Wextra` enables useful compiler warnings and should be used during development.

##  Makefile Basics

Makefile automates builds:

```makefile
program: main.o
	gcc main.o -o program

main.o: main.c
	gcc -c main.c -o main.o

clean:
	rm -f *.o program
```

Core model:

```text
.c + required .h → .o → executable
```

`make` builds; `make clean` removes generated files.

##  OS Connection

C gives direct control over memory, processes, files, and system calls. Understanding compilation, object files, memory size, and command-line arguments forms the foundation for deeper OS programming.

**Next:** Pointers, addresses, dereferencing, and memory.
