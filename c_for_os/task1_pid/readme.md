# Task 1: Process ID and Parent Process ID

## Objective

Write a C program that prints the Process ID (PID) and the Parent Process ID (PPID) of itself, and build it using a Makefile.

## Files

| File | Purpose |
| --- | --- |
| `process.c` | Source code that prints PID and PPID |
| `Makefile` | Automates compiling, running and cleaning |

## Key Concepts

**Process.** A program stored on disk is just a file. When it is loaded into memory and starts executing, it becomes a process.

**PID (Process ID).** A unique integer that the kernel assigns to every running process. It is how the operating system identifies and tracks a process.

**PPID (Parent Process ID).** The PID of the process that created the current process. When you run `./process` from a terminal, the shell (bash) is the parent.

**PCB (Process Control Block).** For every process, the kernel keeps a data structure called the PCB. It stores the PID, PPID, process state, and memory information. The system calls `getpid()` and `getppid()` read these values from the PCB.

## System Calls Used

Both are declared in `<unistd.h>`:

```c
pid_t getpid(void);   // returns the PID of the calling process
pid_t getppid(void);  // returns the PID of the parent process
```

`pid_t` is an integer type. To print it portably, cast it to `long` and use `%ld`.

## Code Explanation

```c
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Process is :%ld \n", (long)getpid());
    printf("Parent of this process is: %ld \n", (long)getppid());
    return 0;
}
```

1. `stdio.h` provides `printf`. `unistd.h` provides `getpid` and `getppid`.
2. `getpid()` switches the CPU into kernel mode, reads the PID from the PCB, and returns it.
3. `getppid()` does the same for the parent's PID.
4. The `(long)` cast together with `%ld` avoids size mismatches on different systems.

## Makefile Explanation

```makefile
CC = gcc
CFLAGS = -Wall -Wextra

process: process.c
	$(CC) $(CFLAGS) -o process process.c

run: process
	./process

clean:
	rm -f process

.PHONY: run clean
```

- `CC` and `CFLAGS` are variables for the compiler and its warning flags.
- `process: process.c` is a rule. The target `process` is rebuilt only when `process.c` changes.
- `run` builds the program if needed and then executes it.
- `clean` removes the executable.
- `.PHONY` tells make that `run` and `clean` are action names, not files.
- Command lines under a target must start with a TAB character. Spaces cause the error `missing separator`.

## Build and Run

```bash
make
./process
```

## Sample Output

```text
Process is :2845
Parent of this process is: 2410
```

The actual numbers will differ on every run.

## Verification

Run `echo $$` in the same terminal. It prints the PID of the shell. This number matches the "Parent of this process" value, which confirms that the shell is the parent.

## Common Mistakes

- Calling `getpid()` twice instead of `getpid()` and `getppid()`, which prints the same number on both lines.
- Using spaces instead of a TAB in the Makefile.
- Printing `pid_t` with `%d` without a cast.