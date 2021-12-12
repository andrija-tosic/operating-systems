/*
Korišćenjem programskog jezika C kreirati program koji se deli u dva Linux procesa koji generišu slučajne celobrojne
vrednosti i upisuju ih u datoteku čiji se naziv prosleđuje kao argument komandne linije glavnog procesa.
Prvi proces generiše niz od tri slučajne pozitivne celobrojne vrednosti i upisuje ih u datoteku. 
Nakon toga drugi proces generiše dve slučajne negativne celobrojne vrednosti i upisuje ih u istu datoteku.
Postupak se ponavlja N puta (naizmenično prvi pa drugi proces). 
Vrednost N se takođe prosleđuje kao argument komandne linije. Sinhronizovati rad procesa korišćenjem semafora.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

union semun {
    int val;
    struct semid_ds* buf;
    ushort* array;
    struct seminfo* __buf;
    void* __pad;
};

int main(int argc, char* argv[])
{
    char* file_name = argv[1];

    int N = atoi(argv[2]);

    union semun semopts;

    struct sembuf sem_wait0 = { 0, -1, 0 };
    // struct sembuf sem_wait1 = { 1, -1, 0 };

    struct sembuf sem_post0 = { 0, 1, 0 };
    // struct sembuf sem_post1 = { 1,  1, 0 };

    //int sems = semget((key_t)1, 2, 0666 | IPC_CREAT); // 2 semafora za prvi i drugi proces

    int sem0 = semget((key_t)0, 1, 0666 | IPC_CREAT);
    int sem1 = semget((key_t)1, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    // prvi semafor inicijalno 1
    semctl(0, 0, SETVAL, semopts);
    semopts.val = 0;
    // drugi semafor inicijalno 0
    semctl(1, 1, SETVAL, semopts);

    FILE* f;

    // ocistiti prethodni sadrzaj fajla
    f = fopen(file_name, "w");
    fclose(f);

    srand(123);

    // proces 1
    if (fork() != 0) {
        int br = 0;
        while (br < N) {
            semop(sem0, &sem_wait0, 1);
            printf("||||||||||||||||||\n");
            {
                int niz[3];
                for (int i = 0; i < 3; i++) {
                    niz[i] = rand() % 101;
                }
                f = fopen(file_name, "a");
                fprintf(f, "%d, %d, %d\n", niz[0], niz[1], niz[2]);
                fclose(f);
            }
            br++;
            semop(sem1, &sem_post0, 1);
        }
        exit(0);
    }
    //proces 2
    else {
        int br = 0;
        while (br < N) {
            semop(sem1, &sem_wait0, 1);
            printf("==================\n");
            {
                int a = rand() % 101;
                int b = rand() % 101;
                f = fopen(file_name, "a");

                fprintf(f, "%d, %d\n", a, b);
                fclose(f);
            }
            br++;
            semop(sem0, &sem_post0, 1);
        }
        exit(0);
    }

    //semctl(sems, 0, IPC_RMID, 0);
    //semctl(sems, 1, IPC_RMID, 0);

    return 0;
}