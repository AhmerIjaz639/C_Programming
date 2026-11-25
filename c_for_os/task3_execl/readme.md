# Task 3: Running a Program using fork() and execl()

## Objective

Create child processes with `fork()` and make each child run the `ls` command using `execl()`. The parent waits for the children to finish.

## Files

| File | Purpose |
| --- | --- |
| `main.c` | Creates two children, each running `ls` |
| `date_task.c` | Child runs `date`, parent prints a message after it finishes |
| `Makefile` | Build rules |

## Key Concepts

**fork() creates, exec replaces.** `fork()` only produces a copy of the current process. To run a different program inside that new process, the `exec` family is used.

**exec family.** An `exec` call erases the current code, data, stack and heap of the process and loads a new executable in its place.

- The PID does not change. It is the same process running a different program.
- On success, `exec` never returns, because the old code no longer exists.
- If the line after `exec` runs, then `exec` has failed.

## Syntax

```c
int execl(const char *path, const char *arg0, const char *arg1, ..., NULL);
```

| Argument | Meaning |
| --- | --- |
| `path` | Full path of the executable, for example `/usr/bin/ls` (find it with `which ls`) |
| `arg0` | Name of the program, by convention the same as the command |
| `arg1...` | Optional arguments such as `-l` |
| `NULL` | Mandatory terminator of the argument list |

The shell command `ls -l` becomes:

```c
execl("/usr/bin/ls", "ls", "-l", NULL);
```

## Code: main.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t childpid;

    childpid = fork();
    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0) {
        execl("/usr/bin/ls", "ls", NULL);
        perror("Child 1 failed to exec ls");
        return 1;
    }

    childpid = fork();
    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0) {
        execl("/usr/bin/ls", "ls", NULL);
        perror("Child 2 failed to exec ls");
        return 1;
    }

    wait(NULL);
    wait(NULL);
    return 0;
}
```

### Explanation

1. The first `fork()` creates Child 1. In the child, `childpid` is `0`, so the `if (childpid == 0)` block runs only in the child.
2. Inside the child, `execl` replaces the program with `ls`. The `perror` line below it runs only if `execl` fails.
3. The parent skips that block and calls `fork()` again to create Child 2, which behaves the same way.
4. The parent calls `wait(NULL)` once per child so that it does not exit early and no zombie processes remain.
5. The directory listing is printed twice, once by each child.

## Code: date_task.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t childpid = fork();

    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0) {
        execl("/bin/date", "date", NULL);
        perror("Failed to exec date");
        return 1;
    }

    wait(NULL);
    printf("Child has finished displaying the date\n");
    return 0;
}
```

The parent prints its message only after `wait()` returns, so the message always appears after the date output.

## Build and Run

```bash
gcc -Wall -Wextra -o main main.c
./main
```

## Common Mistakes

- Forgetting the final `NULL` in `execl`.
- Writing `"ls"/NULL` or any other separator instead of a comma.
- Spelling errors in variable names, such as `childpdi` instead of `childpid`.
- Not calling `wait()` for every child. For many children use `while (wait(NULL) > 0);`.
- Writing code after `execl` and expecting it to run on success.