# Task 2: Process Fan and Process Chain using fork()

## Objective

Use the `fork()` system call to create multiple processes from a command line argument `n`, first as a process fan and then as a process chain. Each process prints its own information.

## Files

| File | Purpose |
| --- | --- |
| `output.c` | Process fan program |
| `chain.c` | Process chain program |
| `Makefile` | Build rules |

## Key Concepts

**fork().** Creates a new process that is a copy of the calling process. The original is the parent and the new one is the child. Both continue executing from the line after `fork()`.

**Return values.** `fork()` is called once but returns twice, with a different value in each process:

| Process | Return value |
| --- | --- |
| Child | `0` |
| Parent | PID of the child (greater than 0) |
| Error | `-1` |

**Process fan.** One parent creates all the children. The children do not create any further processes.

```
        Parent
      /   |   \
 Child1 Child2 Child3
```

**Process chain.** Each child creates the next child.

```
Parent -> Child1 -> Child2 -> Child3
```

**Zombie process.** A child that has finished but has not been waited on by its parent stays in the process table as a zombie. The parent prevents this by calling `wait()`.

## Process Fan Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t childpid = 0;
    int i, n;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s process\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n < 1) {
        fprintf(stderr, "n must be 1 or more\n");
        return 1;
    }

    for (i = 1; i < n; i++) {
        childpid = fork();
        if (childpid <= 0)
            break;
    }

    fprintf(stderr, "i:%d process ID:%ld parent ID:%ld child ID:%ld\n",
            i, (long)getpid(), (long)getppid(), (long)childpid);

    if (childpid > 0) {
        while (wait(NULL) > 0);
    }

    return 0;
}
```

### Explanation

1. `argc != 2` checks that the user passed exactly one argument. The program returns immediately if not, to avoid reading a missing `argv[1]`.
2. `atoi` converts the argument string to an integer, and `n < 1` rejects invalid values.
3. The loop runs `n - 1` times, so with the original parent there are `n` processes in total.
4. `if (childpid <= 0) break;` makes the child leave the loop right after it is created. Only the parent keeps looping, so only the parent creates children. This is what makes it a fan.
5. Every process prints its loop index, its PID, its parent's PID and the value of `childpid`.
6. The parent calls `wait(NULL)` in a loop until no children remain, which prevents zombies.

### Dry Run with n = 4

| Process | i when it leaves the loop | childpid |
| --- | --- | --- |
| Child 1 | 1 | 0 |
| Child 2 | 2 | 0 |
| Child 3 | 3 | 0 |
| Parent | 4 | PID of the last child |

## Process Chain Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t childpid = 0;
    int i, n;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n < 1) {
        fprintf(stderr, "n must be 1 or more\n");
        return 1;
    }

    for (i = 1; i < n; i++) {
        childpid = fork();
        if (childpid == -1) {
            perror("Failed to fork");
            return 1;
        }
        if (childpid > 0)
            break;
    }

    fprintf(stderr, "i:%d process ID:%ld parent ID:%ld child ID:%ld\n",
            i, (long)getpid(), (long)getppid(), (long)childpid);

    if (childpid > 0)
        wait(NULL);

    return 0;
}
```

### Explanation

The only important change is the break condition. `if (childpid > 0) break;` stops the parent after it creates one child. The child (where `childpid` is 0) stays in the loop and creates the next child. Each process waits for its single child.

## Fan vs Chain Summary

| | Fan | Chain |
| --- | --- | --- |
| Break condition | `if (childpid <= 0) break;` | `if (childpid > 0) break;` |
| Who creates children | Only the original parent | Every new child |
| Shape | One level | A line of processes |

## Build and Run

```bash
gcc -Wall -Wextra -o output output.c
./output 4
```

## Common Mistakes

- Missing `return 1;` after the argument check, which crashes when `argv[1]` is absent.
- Using the wrong break condition, which accidentally creates a chain instead of a fan or the reverse.
- Forgetting `wait()`, so the parent may exit before its children finish.