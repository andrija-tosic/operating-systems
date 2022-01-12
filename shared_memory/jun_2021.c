#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#define A_KEY 100001
#define B_KEY 100002
#define MEM_KEY 10003
union semun
{
 int val;
 struct semid_ds *buf;
 unsigned short *array;
 struct seminfo * __buf;
 void * __pad;
};

int main() {
    int semA, semB;
    int memid;
    char* shmem; //pokazivac za mapiranje deljene memorije u adresni procesa
    int i;
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};
    union semun opts;

    semA = semget(A_KEY, 1, IPC_CREAT | 0666);
    semB = semget(B_KEY, 1, IPC_CREAT | 0666);

    opts.val = 1;
    semctl(semA, 0, SETVAL, opts);

    opts.val = 0;
    semctl(semB, 0, SETVAL, opts);

    memid = shmget(MEM_KEY, 80*sizeof(char), IPC_CREAT | 0666);

    if (fork() != 0) {
        shmem = shmat(memid, NULL, 0);
        do {
            semop(semA, &lock, 1);
            printf("Reading A: ");
            gets(shmem);
            semop(semB, &unlock, 1);
        } while (strcmp(shmem, "KRAJ") != 0);
        wait(NULL);
    }
    else {
        shmem = shmat(memid, NULL, 0);
        while (1) {
            semop(semB, &lock, 1);
            printf("Printing B: %s\n", shmem);
            semop(semA, &unlock, 1);
        }
    }
    return 0;
}