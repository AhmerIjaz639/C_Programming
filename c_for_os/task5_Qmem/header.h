#ifndef TASK01_H
#define TASK01_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>

#define PERM (0600 )
#define KEY 123

typedef struct {
    long mtype;
    char mtext[100];
} mymsg_t;

#endif