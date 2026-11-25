#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define PERM (0666 | IPC_CREAT)


struct student {
    int rollno;
    float gpa;
};

int main(void) {
    key_t mykey = 5555;

    int id = shmget(mykey, sizeof(struct student), PERM);
    if (id == -1) {
        perror("shmget failed");
        return 1;
    }


    struct student *sptr = (struct student *)shmat(id, NULL, 0);
    if (sptr == (void *)-1) {
        perror("shmat failed");
        return 1;
    }


    sptr->rollno = 101;
    sptr->gpa = 3.85;

    printf("Writer: Wrote Student [Roll No: %d, GPA: %.2f] to Shared Memory\n", 
            sptr->rollno, sptr->gpa);

    
    shmdt(sptr);

    return 0;
}