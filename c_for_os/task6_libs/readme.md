# Task 6: Static Library (libcalc and libmymath)

## Objective

Build a static library from C source files, link it into a program, and automate the whole process with a Makefile.

## Files

| File | Purpose |
| --- | --- |
| `mymath.h` | Header with function prototypes |
| `mymath.c` | Function implementations (`add`, `multiply`) |
| `main.c` | Program that uses the library |
| `Makefile` | Builds objects, the library and the executable |

The same steps apply to the practice library `libcalc.a`, which contains `subtract` and `power`.

## Key Concepts

**Library.** A collection of precompiled functions that many programs can reuse without copying source code.

**Static library.** An archive file with the extension `.a`. At link time the needed code is copied into the final executable, so the executable runs without the library file.

**Naming rule.** A static library file must start with `lib`. A library called `mymath` is stored in `libmymath.a`.

**Header file.** Contains only declarations (prototypes). It tells the compiler what functions exist. The definitions live in the `.c` file and end up in the library.

## Build Pipeline

```
mymath.c --gcc -c--> mymath.o --ar rcs--> libmymath.a
                                              |
main.c   --gcc -c--> main.o  --------gcc link-+--> myprogram
```

## Commands

```bash
gcc -Wall -Wextra -c mymath.c                       # compile to object file
ar rcs libmymath.a mymath.o                         # create the static library
gcc -Wall -Wextra -c main.c                         # compile main to object file
gcc -Wall -Wextra -o myprogram main.o -L. -lmymath  # link everything
```

| Item | Meaning |
| --- | --- |
| `-c` | Compile only, produce an object file, do not link |
| `ar` | The archiver tool |
| `r` | Insert or replace files in the archive |
| `c` | Create the archive if it does not exist |
| `s` | Write an index (symbol table) for faster linking |
| `-L.` | Search for libraries in the current directory |
| `-lmymath` | Link with `libmymath.a` (drop the `lib` prefix and `.a` suffix) |

For a library named `libcrypto.a`, the link flag is `-lcrypto`.

## Source Code

mymath.h

```c
#ifndef MYMATH_H
#define MYMATH_H

int add(int a, int b);
int multiply(int a, int b);

#endif
```

mymath.c

```c
#include "mymath.h"

int add(int a, int b)      { return a + b; }
int multiply(int a, int b) { return a * b; }
```

main.c

```c
#include <stdio.h>
#include "mymath.h"

int main(void) {
    printf("5 + 3 = %d\n", add(5, 3));
    printf("5 * 3 = %d\n", multiply(5, 3));
    return 0;
}
```

## Makefile Explanation

```makefile
CC = gcc
CFLAGS = -Wall -Wextra

myprogram: main.o libmymath.a
	$(CC) $(CFLAGS) -o myprogram main.o -L. -lmymath

libmymath.a: mymath.o
	ar rcs libmymath.a mymath.o

main.o: main.c mymath.h
	$(CC) $(CFLAGS) -c main.c

mymath.o: mymath.c mymath.h
	$(CC) $(CFLAGS) -c mymath.c

run: myprogram
	./myprogram

clean:
	rm -f main.o mymath.o libmymath.a myprogram

.PHONY: run clean
```

- Each rule lists its dependencies. Make rebuilds a target only when a dependency is newer.
- Running `make` builds `myprogram`, which first triggers `libmymath.a`, which first triggers `mymath.o`.
- Header files are listed as dependencies so a change to `mymath.h` forces a rebuild.
- Command lines must begin with a TAB.

## Build and Run

```bash
make
make run
make clean
```

## Expected Output

```text
5 + 3 = 8
5 * 3 = 15
```

## Practice Variant: libcalc.a

Library functions:

```c
int subtract(int a, int b);        // returns a - b
int power(int base, int exp);      // returns base raised to exp
```

The main program calls `subtract(10, 4)` and `power(2, 3)` and prints 6 and 8. The Makefile follows the same pattern, replacing `mymath` with `calc` and linking with `-L. -lcalc`.

To inspect the contents of a library:

```bash
ar t libmymath.a
nm libmymath.a
```

## Common Mistakes

- Naming the library without the `lib` prefix, so `-l` cannot find it.
- Writing `-lmymath.a` or `-llibmymath` instead of `-lmymath`.
- Forgetting `-L.` when the library is in the current folder.
- Placing `-lmymath` before `main.o` in the link command. The library must come after the object files that need it.
- Putting function bodies in the header instead of the `.c` file.