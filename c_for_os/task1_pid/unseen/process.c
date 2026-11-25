#include <unistd.h>
#include <stdio.h>

int main(void){
    printf("process id : %ld \n",(long)getpid());
    printf("parent process id : %ld \n",(long)getppid());
    if (getpid()% 2 == 0){
        printf("even process");
    }
    else{
        printf("odd process");
    }
}