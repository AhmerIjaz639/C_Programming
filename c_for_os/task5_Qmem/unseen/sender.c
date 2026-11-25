#include "header.h"

int main(void){
    int qid;
    mymsg_t msg;

    qid=msgget(KEY,PERM|IPC_CREAT);
    if(qid==-1){
        perror("failed to create a Q");
        return 1;
    }

    for (int i =1;i<=5;i++){
        msg.mtype=9;
        snprintf(msg.mtext,sizeof(msg.mtext),"message %d from sender with urgent priority!!",i);
        if(msgsnd(qid,&msg,strlen(msg.mtext)+1,0)==-1){
            perror("failed to send msg ");
            return 1;
        }
        printf("Sender sent : %s\n",msg.mtext);


    }
    
    for (int i =1;i<=2;i++){
        msg.mtype=1;
         snprintf(msg.mtext,sizeof(msg.mtext),"message %d from sender with normal  priority!!",i);
        if(msgsnd(qid,&msg,strlen(msg.mtext)+1,0)==-1){
            perror("failed to send msg ");
            return 1;
        }
         printf("Sender sent : %s\n",msg.mtext);

    }
}