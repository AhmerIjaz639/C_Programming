#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<sys/time.h>

#define PERM (0666 | IPC_CREAT)
int main(void){
    int id;
    key_t mykey=1234;
    char *cptr;

    id=shmget(mykey,100,PERM);
     if (id == -1) {
        perror("failed to get shared memory segment");
        return 1;
    }
    
    cptr = (char *)shmat(id, NULL, SHM_RDONLY);
    if (cptr == (char *)-1) {
        perror("failed to attach shared memory segment");
        return 1;
    }
     for (int i = 0; i < 5; i++) {
        if (cptr[0] == '\0')
            printf("Reader found shared memory empty\n");
        else
            printf("Reader read from shared memory: %s", cptr);
        sleep(5);
    }
    
    
    return 0;
}