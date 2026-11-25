#include "header.h"
int main(void){
    int qid;
    mymsg_t msg;

    qid=msgget(KEY,PERM | IPC_CREAT);
    if(qid==-1){
        perror("failed to create msg Q");
        return 1;
    }
    for (int i=1;i<=3;i++){
        msg.mtype=1;
        snprintf(msg.mtext,sizeof(msg.mtext),"Message %d from sender",i);
        if(msgsnd(qid,&msg,strlen(msg.mtext)+1,0)==-1){
            perror("failed to send msg");
            return 1;
        }
        printf("sender sent : %s\n",msg.mtext);
    }
    return 0;

}