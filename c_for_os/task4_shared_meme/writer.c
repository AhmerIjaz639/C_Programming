#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<sys/time.h>

#define PERM (0666 | IPC_CREAT)
int main(void){

    int id;
    char *cptr;
    key_t mykey=1234;
    struct timeval tv;

    id=shmget(mykey, 100, PERM);
    if(id==-1){
        perror("Failed to create shared memory");
        return 1;
    }

    cptr=(char *)shmat(id,NULL,0);
    if(cptr==(char *)-1){
        perror("failed to attach shared memory");
        return 1;
    }

    for (int i=0;i<5;i++){
        gettimeofday(&tv,NULL);
        snprintf(cptr,100,"Current time in seconds :%ld , Microseconds : %ld\n",
        tv.tv_sec,tv.tv_usec);
        printf("writer wrote in shared memory :%s",cptr);
        sleep(5);
    }

    return 0;
}