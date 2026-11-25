#include "header.h"
int main(void){
    int qid;
    mymsg_t msg;

    qid=msgget(KEY,PERM | IPC_CREAT);
    if(qid==-1){
        perror("failed to get msg q ");
        return 1;
    }

    for(int i=1;i<=3;i++){
        if(msgrcv(qid,&msg,sizeof(msg.mtext),1,0)==-1){
            perror("failed to rcv msg");
            return 1;
        }
        printf("rcv got : %s\n",msg.mtext);
     
    }
    return 0;
}