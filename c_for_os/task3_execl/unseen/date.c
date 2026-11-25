#include <unistd.h>
#include<stdio.h>
#include<sys/wait.h>

int main(void){
    pid_t childpid;

    childpid=fork();
    if(childpid==-1){
        perror("failed to fork ");
        return 1;
    }

    
    if(childpid==0){
        execl("/usr/bin/date","date",NULL);
        perror("child 1 failed to exec ls ");
        return 1;
    }

    wait(NULL);
    printf("child 1 ends at this moment: ");
    return 0;
   
}