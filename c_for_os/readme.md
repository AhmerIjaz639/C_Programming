# C for Operating Systems: Lab Practice

A collection of C programs that demonstrate core Linux operating system concepts: processes, process creation, program execution, inter-process communication, and libraries. Each task has its own folder with source code and a detailed README.

## Download

All tasks, code and explanations are also available as a single PDF for offline revision.

[![Download PDF](https://img.shields.io/badge/Download-PDF-blue?style=for-the-badge)](OS_Lab_Codes_Before_Mids.pdf?raw=true)

Direct link: [OS_Lab_Codes_Before_Mids.pdf](OS_Lab_Codes_Before_Mids.pdf?raw=true)

## Repository Structure
# Before Mids 
| Folder | Topic | Main System Calls / Tools |
| --- | --- | --- |
| Task 1 | Process ID and Parent ID, Makefile | `getpid`, `getppid`, `make` |
| Task 2 | Process fan and process chain | `fork`, `wait` |
| Task 3 | Running programs in child processes | `fork`, `execl`, `wait` |
| Task 4 | Shared memory | `shmget`, `shmat`, `shmdt` |
| Task 5 | Message queues | `msgget`, `msgsnd`, `msgrcv` |
| Task 6 | Static library | `gcc -c`, `ar`, `-L`, `-l` |

## Requirements

- Linux or WSL
- `gcc`
- `make`

## Concepts

### 1. Programs and Processes

A program is an executable file on disk. A process is a program that is running in memory. The kernel tracks each process using a Process Control Block (PCB), which stores the PID, parent PID, state and memory information.

- **PID:** unique number identifying a process.
- **PPID:** PID of the process that created it. For programs started from a terminal, the parent is the shell.

### 2. System Calls and Kernel Mode

A system call is a request from a user program to the kernel. When one is made, the CPU switches from user mode to kernel mode, the kernel performs the work, and control returns to the program. Everything in this repository (`fork`, `exec`, `shmget`, `msgsnd`) is a system call or a thin wrapper around one.

### 3. Process Creation with fork()

`fork()` creates a copy of the calling process. It is called once and returns twice:

| Where | Return value |
| --- | --- |
| Child | `0` |
| Parent | Child's PID |
| Failure | `-1` |

Parent and child continue from the same point but have separate memory.

**Process fan:** the parent creates all children (`if (childpid <= 0) break;`).
**Process chain:** each child creates the next one (`if (childpid > 0) break;`).

### 4. Waiting and Zombie Processes

When a child exits, the kernel keeps a small record of it until the parent collects its exit status. If the parent never does, the child remains a zombie. `wait()` collects the status. To wait for all children use:

```c
while (wait(NULL) > 0);
```

### 5. Executing Programs with exec

The `exec` family replaces the current process image with a new program. The PID stays the same. On success `exec` never returns; code after it runs only on failure. The usual pattern is `fork()` followed by `exec` in the child and `wait()` in the parent, which is how a shell runs commands.

```c
execl("/usr/bin/ls", "ls", "-l", NULL);   // NULL terminates the list
```

### 6. Inter-Process Communication (IPC)

Processes are isolated from each other, so the kernel provides IPC mechanisms. This repository covers two System V mechanisms.

| Feature | Shared Memory | Message Queue |
| --- | --- | --- |
| Data model | Raw block of memory | Discrete typed messages |
| Speed | Fastest | Slower (copied through kernel) |
| Synchronization | None built in | Receiver blocks until a message arrives |
| Selection | Not applicable | Receive by message type |
| Key calls | `shmget`, `shmat`, `shmdt` | `msgget`, `msgsnd`, `msgrcv` |

**Shared memory** maps one kernel-managed segment into several processes. Both sides use the same key. A struct can be stored directly by casting the attached pointer.

**Message queues** store messages in the kernel. The message structure must begin with `long mtype`. The size given to `msgsnd` and `msgrcv` is the size of the data only. The type allows selective receiving, which gives a simple priority mechanism.

Both are persistent: they remain in the kernel after the programs exit and must be removed manually.

```bash
ipcs -m / ipcrm -M <key>     # shared memory
ipcs -q / ipcrm -Q <key>     # message queue
```

### 7. Libraries and the Build Process

A static library is an archive (`.a`) of object files. Its code is copied into the executable at link time.

```bash
gcc -c mymath.c                          # source to object
ar rcs libmymath.a mymath.o              # object to archive
gcc -o prog main.o -L. -lmymath          # link
```

The file must be named `lib<name>.a` and linked with `-l<name>`.

### 8. Makefiles

A Makefile describes targets, their dependencies and the commands to build them. Make rebuilds only what is out of date. Recipe lines must start with a TAB. `.PHONY` marks targets such as `run` and `clean` that do not correspond to files.

## Quick Reference

| Need | Command |
| --- | --- |
| Compile with warnings | `gcc -Wall -Wextra -o out file.c` |
| Find a program path | `which ls` |
| Show shell PID | `echo $$` |
| List processes | `ps`, `ps aux` |
| List shared memory | `ipcs -m` |
| Remove shared memory | `ipcrm -M <key>` or `ipcrm -m <shmid>` |
| List message queues | `ipcs -q` |
| Remove message queue | `ipcrm -Q <key>` |
| List archive contents | `ar t libname.a` |

## Common Error Checklist

- Always check return values: `fork() == -1`, `shmget() == -1`, `shmat() == (void *)-1`, `msgget() == -1`.
- Always `return 1;` after an argument validation failure.
- Always terminate `execl` arguments with `NULL`.
- Always `wait()` for every child.
- Use TAB characters in Makefiles.
- Remove IPC objects after testing.
- Print `pid_t` values with `(long)` and `%ld`.

## Viva Questions

1. Why does `fork()` return twice?
2. What is the difference between a process fan and a process chain?
3. What happens to the PID when `exec` is called?
4. Why does code after a successful `exec` never run?
5. Why does shared memory still exist after the writer exits?
6. Why does `msgsnd` not include `mtype` in the size?
7. What do `-L` and `-l` do in a link command?
8. What does `ar rcs` do?
9. What is a zombie process and how is it avoided?
10. Why is shared memory faster than a message queue, and what does it lack?

## How to Build Any Task

```bash
cd <task folder>
make            # if a Makefile is present
# or
gcc -Wall -Wextra -o program program.c
./program
```

Each task folder contains its own README with a full explanation of the code.
