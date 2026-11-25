# Task 4: Shared Memory (Writer and Reader)

## Objective

Use System V shared memory so that two independent processes can exchange data. Part 1 shares a text string (system time). Part 2 shares a C structure (student record).

## Files

| File | Purpose |
| --- | --- |
| `writer.c` | Writes the current time into shared memory every 5 seconds |
| `reader.c` | Reads the shared memory every 5 seconds |
| `student_writer.c` | Writes a `struct student` into shared memory |
| `student_reader.c` | Reads the `struct student` from shared memory |

## Key Concepts

**Process isolation.** Each process has its own virtual memory. One process cannot directly read the variables of another.

**Inter-Process Communication (IPC).** The set of mechanisms the kernel provides so processes can exchange data.

**Shared memory.** A segment of RAM that the kernel maps into the address space of several processes. It is the fastest IPC mechanism because data is not copied through the kernel on every access.

**Key.** An integer (for example `1234`) agreed upon by both programs. Both use the same key to find the same segment.

## System Calls

Headers: `<sys/ipc.h>`, `<sys/shm.h>`, `<sys/stat.h>`

```c
int shmget(key_t key, size_t size, int shmflg);
void *shmat(int shmid, const void *shmaddr, int shmflg);
int shmdt(const void *shmaddr);
```

| Call | Purpose | Notes |
| --- | --- | --- |
| `shmget` | Create or open a segment | `IPC_CREAT` creates it if it does not exist. Returns the segment ID or `-1` |
| `shmat` | Attach the segment to the process | Pass `NULL` for the address. Flag `0` is read/write, `SHM_RDONLY` is read-only. Returns `(void *)-1` on failure |
| `shmdt` | Detach the segment | Does not delete the segment |

## Code: Time Writer

```c
#define PERM (S_IRUSR | S_IWUSR | IPC_CREAT)

id = shmget(mykey, 100, PERM);
cptr = (char *)shmat(id, NULL, 0);

for (int i = 0; i < 5; i++) {
    gettimeofday(&tv, NULL);
    snprintf(cptr, 100, "current time is seconds: %ld, Microseconds: %ld\n",
             tv.tv_sec, tv.tv_usec);
    printf("Writer wrote in shared memory: %s", cptr);
    sleep(5);
}
```

1. `shmget` creates a 100 byte segment with read and write permission for the owner.
2. `shmat` returns a pointer to that segment. The program treats it as a `char *`.
3. `gettimeofday` fills `struct timeval` with seconds and microseconds.
4. `snprintf` writes the formatted text directly into shared memory, limited to 100 bytes.
5. The loop repeats five times with a 5 second pause.

## Code: Time Reader

```c
id = shmget(mykey, 100, PERM);
cptr = (char *)shmat(id, NULL, SHM_RDONLY);

for (int i = 0; i < 5; i++) {
    if (cptr[0] == '\0')
        printf("Reader found shared memory empty\n");
    else
        printf("Reader read from shared memory: %s", cptr);
    sleep(5);
}
```

1. The reader uses the same key `1234`, so it finds the same segment.
2. `SHM_RDONLY` attaches it read-only, which prevents the reader from modifying the data.
3. If the first byte is `'\0'`, nothing has been written yet.

## Code: Struct Version

Because shared memory is raw memory, a structure can be stored in it directly. No string formatting is needed.

```c
struct student {
    int rollno;
    float gpa;
};
```

Writer:

```c
int id = shmget(5555, sizeof(struct student), 0666 | IPC_CREAT);
struct student *sptr = (struct student *)shmat(id, NULL, 0);

sptr->rollno = 101;
sptr->gpa = 3.85;
shmdt(sptr);
```

Reader:

```c
int id = shmget(5555, sizeof(struct student), 0666);
struct student *sptr = (struct student *)shmat(id, NULL, SHM_RDONLY);

printf("Roll Number : %d\n", sptr->rollno);
printf("GPA         : %.2f\n", sptr->gpa);
shmdt(sptr);
```

Key points:

- The segment size is `sizeof(struct student)`.
- The attached pointer is cast to `struct student *` instead of `char *`.
- Fields are accessed with the arrow operator `->`.
- Both programs must use the same structure definition and the same key.

## Build and Run

```bash
gcc -Wall -Wextra -o writer writer.c
gcc -Wall -Wextra -o reader reader.c
./writer &
./reader
```

## Inspecting and Removing Shared Memory

System V shared memory lives in the kernel. It stays there after the processes exit until it is explicitly removed or the system reboots.

```bash
ipcs -m              # list shared memory segments
ipcrm -M 1234        # remove by key
ipcrm -m <shmid>     # remove by segment ID
```

In code, a segment can be removed with `shmctl(id, IPC_RMID, NULL)`.

Note that `ipcs` displays keys in hexadecimal. Key `1234` appears as `0x000004d2`.

## Limitation

Shared memory has no built-in synchronization. If a reader reads while the writer is halfway through writing, it can see incomplete data. Semaphores or message queues are used to solve this.

## Common Mistakes

- Treating the shared segment as a string when it holds a struct, or the reverse.
- Forgetting to check `shmat` against `(void *)-1`.
- Believing that `shmdt` deletes the segment. It only detaches it.
- Leaving old segments behind and not cleaning them with `ipcrm`.