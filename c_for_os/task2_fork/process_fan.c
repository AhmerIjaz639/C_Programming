#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t childpid = 0;
    int i, n;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n < 1) {
        fprintf(stderr, "n must be 1 or more\n");
        return 1;
    }

    for (i = 1; i < n; i++) {
        childpid = fork();
        if (childpid < 0) {
            perror("fork");
            break;
        }
        if (childpid == 0) {
            break;
        }
    }

    fprintf(stderr,
            "i:%d process id:%ld parent id:%ld child id:%ld\n",
            i, (long)getpid(), (long)getppid(), (long)childpid);

    if (childpid > 0) {
        while (wait(NULL) > 0)
            ;
    }
    return 0;
}