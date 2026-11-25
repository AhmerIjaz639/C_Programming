#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define PERM 0666

struct student {
    int rollno;
    float gpa;
};

int main(void) {
    key_t mykey = 5555;

    int id = shmget(mykey, sizeof(struct student), PERM);
    if (id == -1) {
        perror("shmget failed (Reader)");
        return 1;
    }

   
    struct student *sptr = (struct student *)shmat(id, NULL, SHM_RDONLY);
    if (sptr == (void *)-1) {
        perror("shmat failed");
        return 1;
    }

   
    printf("Reader: Retrieved Student Data:\n");
    printf("   Roll Number : %d\n", sptr->rollno);
    printf("   GPA         : %.2f\n", sptr->gpa);

  
    shmdt(sptr);

    return 0;
}