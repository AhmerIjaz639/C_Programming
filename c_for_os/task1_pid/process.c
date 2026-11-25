#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(void){
    printf("process is :%ld \n", (long)getpid());
    printf("parent process : %ld \n" , (long)getppid());
    return 0; 
}