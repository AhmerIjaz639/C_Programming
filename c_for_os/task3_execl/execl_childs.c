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
        execl("/usr/bin/ls","ls",NULL);
        perror("child 1 failed to exec ls ");
        return 1;
    }

    childpid=fork();
    if(childpid==-1){
        perror("failed to fork ");
        return 1;
    }

     if(childpid==0){
        execl("/usr/bin/mkdir","mkdir","unseen",NULL);
        perror("child 1 failed to exec ls ");
        return 1;
    }

     childpid=fork();
    if(childpid==-1){
        perror("failed to fork ");
        return 1;
    }

     if(childpid==0){
        execl("/usr/bin/touch","touch","date.c",NULL);
        perror("child 1 failed to exec ls ");
        return 1;
    }

     childpid=fork();
    if(childpid==-1){
        perror("failed to fork ");
        return 1;
    }

     if(childpid==0){
        execl("/usr/bin/mv","mv","date.c","unseen",NULL);
        perror("child 1 failed to exec ls ");
        return 1;
    }

   while(wait(NULL)>0);   // i make 4 childs so instead of writing wait 4 times i use this loop 
    return 0; 
    
}