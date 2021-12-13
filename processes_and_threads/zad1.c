/*
Korišćenjem programskog jezika C kreirati program koji se deli u dva Linux procesa koji 
generišu slučajne celobrojne vrednosti i upisuju ih u datoteku čiji se naziv prosleđuje kao 
argument komandne linije glavnog procesa. Prvi proces generiše niz od tri slučajne pozitivne 
celobrojne vrednosti i upisuje ih u datoteku. Nakon toga drugi proces generiše dve slučajne 
negativne celobrojne vrednosti i upisuje ih u istu datoteku. Postupak se ponavlja N puta 
(naizmenično prvi pa drugi proces). Vrednost N se takođe prosleđuje kao argument komandne 
linije. Sinhronizovati rad procesa korišćenjem semafora.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/sem.h>

union semun
{
    int val;
    struct semid_ds* buf;
    ushort* array;
};

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        exit(1);
    }

    srand(time(NULL));
    FILE* file;
    int N = atoi(argv[2]);

    union semun semopts;

    struct sembuf sem_wait = { 0, -1, 0 };
    struct sembuf sem_post = { 0, 1, 0 };

    int mutex1 = semget((key_t)0, 1, 0666 | IPC_CREAT);
    int mutex2 = semget((key_t)1, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    semctl(mutex1, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(mutex2, 0, SETVAL, semopts);

    if (fork() != 0)
    {
        int niz[3];
        int br = 0;

        int m = N / 2 == 0 ? N / 2 : N / 2 + 1;

        while(br < m)
        {
            semop(mutex1, &sem_wait, 1);
            for(int i = 0; i < 3; i++)
            {
                niz[i] = rand() % 100;
            }

            file = fopen(argv[1], "a");
            fprintf(file, "%d %d %d\n", niz[0], niz[1], niz[2]);
            fclose(file);

            br++;
            semop(mutex2, &sem_post, 1);
        }
    }
    else
    {
        int niz[2];
        int br = 0;

        while(br < N / 2)
        {
            semop(mutex2, &sem_wait, 1);
            for(int i = 0; i < 2; i++)
            {
                niz[i] = - rand() % 100;
            }

            file = fopen(argv[1], "a");
            fprintf(file, "%d %d\n", niz[0], niz[1]);
            fclose(file);

            br++;
            semop(mutex1, &sem_post, 1);
        }
    }
}