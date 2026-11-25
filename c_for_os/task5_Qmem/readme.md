# Task 5: Message Queue (Sender and Receiver)

## Objective

Use System V message queues so that a sender process can pass messages to a receiver process through the kernel. Includes a priority example using message types.

## Files

| File | Purpose |
| --- | --- |
| `task01.h` | Shared header: key, permissions and message structure |
| `sender.c` | Sends messages to the queue |
| `receiver.c` | Receives messages from the queue |
| `priority_sender.c` | Sends type 1 messages followed by a type 9 message |
| `priority_receiver.c` | Receives the type 9 message first |

## Key Concepts

**Message queue.** A list of messages stored inside the kernel. A sender adds messages and a receiver removes them. The kernel holds them, so the receiver does not need to be running when the message is sent.

**Compared with shared memory.** A message queue transfers whole messages and the receiver blocks until a message is available. This avoids the problem of reading half-written data.

**Message type (`mtype`).** Every message has a positive integer type. Receivers can ask for a specific type, so `mtype` works as a category or priority label.

## Message Structure

```c
typedef struct {
    long mtype;       // must be the first member and greater than 0
    char mtext[100];  // message data
} mymsg_t;
```

The first member must be a `long` named by convention `mtype`. The rest of the structure is the user data.

## System Calls

Headers: `<sys/ipc.h>`, `<sys/msg.h>`

```c
int msgget(key_t key, int msgflg);
int msgsnd(int qid, const void *msgp, size_t msgsz, int msgflg);
ssize_t msgrcv(int qid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```

| Call | Purpose |
| --- | --- |
| `msgget` | Create or open a queue. Use `PERM \| IPC_CREAT`. Returns the queue ID or `-1` |
| `msgsnd` | Send a message. `msgsz` is the size of the data only, not including `mtype` |
| `msgrcv` | Receive a message. Blocks if no matching message exists |

`msgtyp` rules for `msgrcv`:

| Value | Behavior |
| --- | --- |
| `0` | Take the first message in the queue (FIFO) |
| greater than 0 | Take the first message of exactly that type |

## Code: task01.h

```c
#ifndef TASK01_H
#define TASK01_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define PERM (S_IRUSR | S_IWUSR)
#define KEY 123

typedef struct {
    long mtype;
    char mtext[100];
} mymsg_t;

#endif
```

The include guard (`#ifndef`) prevents the header from being included twice. Both programs share this file so they agree on the key and the message layout.

## Code: sender.c

```c
#include "task01.h"

int main(void) {
    int qid;
    mymsg_t msg;

    qid = msgget(KEY, PERM | IPC_CREAT);
    if (qid == -1) {
        perror("Failed to create message queue");
        return 1;
    }

    for (int i = 1; i <= 3; i++) {
        msg.mtype = 1;
        snprintf(msg.mtext, sizeof(msg.mtext), "Message %d from sender", i);
        if (msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
            perror("Failed to send message");
            return 1;
        }
        printf("Sender sent: %s\n", msg.mtext);
    }
    return 0;
}
```

1. `msgget` creates the queue (or opens it if it exists).
2. Each loop iteration sets the type, fills the text and calls `msgsnd`.
3. The size passed is `strlen(msg.mtext) + 1`, which is only the text including the terminating null character.

## Code: receiver.c

```c
#include "task01.h"

int main(void) {
    int qid;
    mymsg_t msg;

    qid = msgget(KEY, PERM | IPC_CREAT);
    if (qid == -1) {
        perror("Failed to get message queue");
        return 1;
    }

    for (int i = 1; i <= 3; i++) {
        if (msgrcv(qid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
            perror("Failed to receive message");
            return 1;
        }
        printf("Receiver got: %s\n", msg.mtext);
    }
    return 0;
}
```

`msgrcv` asks for type `1` and blocks until such a message exists.

## Priority Example

The sender sends two normal log messages (type 1) and then one urgent alert (type 9):

```c
msg.mtype = 1;
snprintf(msg.mtext, sizeof(msg.mtext), "INFO: User logged in");
msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0);   // sent twice

msg.mtype = 9;
snprintf(msg.mtext, sizeof(msg.mtext), "URGENT: Fire in server room");
msgsnd(qid, &msg, strlen(msg.mtext) + 1, 0);
```

The receiver asks for type 9 first, then reads the remaining type 1 messages:

```c
msgrcv(qid, &msg, sizeof(msg.mtext), 9, 0);    // urgent alert first
msgrcv(qid, &msg, sizeof(msg.mtext), 1, 0);    // then the logs
msgrcv(qid, &msg, sizeof(msg.mtext), 1, 0);
```

Although the urgent message was sent last, it is received first because the receiver selects by type.

## Build and Run

```bash
gcc -Wall -Wextra -o sender sender.c
gcc -Wall -Wextra -o receiver receiver.c
./sender
./receiver
```

## Inspecting and Removing the Queue

```bash
ipcs -q          # list message queues
ipcrm -Q 123     # remove queue by key
```

Like shared memory, a message queue remains in the kernel after the programs exit.

## Common Mistakes

- Passing `sizeof(mymsg_t)` as `msgsz`. It must be the size of the data only (`sizeof(msg.mtext)` or the string length plus one).
- Placing `mtype` anywhere other than the first member, or using a value of 0 or less.
- Using different keys or different struct definitions in the sender and receiver.
- Forgetting to remove old queues, which leaves unread messages for the next run.