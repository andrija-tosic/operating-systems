#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define true 1
#define false 0

int niz[] = {1, 2, 3, 4, 5};

typedef char bool;

pthread_mutex_t mutex;
pthread_cond_t suma_cond;
pthread_cond_t printed_cond;
bool parna_suma = false; // jako bitno
bool printed = true; // jako bitno

void* writeThread(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        {
            while (!printed) {
                pthread_cond_wait(&printed_cond, &mutex);
            }

            niz[rand() % 5] = rand() % 21 - 10;

            int sum=0;
            for (int i=0; i<5; i++)
                sum += niz[i];

            printf("Thread(%c)\n", (char*)arg);

            if (sum % 2 == 0) {
                parna_suma = true;
                printed = false;
                pthread_cond_signal(&suma_cond);
            }
        }
        pthread_mutex_unlock(&mutex);

        sleep(3);
    }
}

void* printThread(void* arg) {
    while(true) {
        pthread_mutex_lock(&mutex);
        {
            while (!parna_suma) {
                pthread_cond_wait(&suma_cond, &mutex);
            }

            printf("Suma parna: ");

            for (int i=0; i<5; i++) {
                printf("%d ", niz[i]);
            }
            puts("");
            parna_suma = false;
            printed = true;
            pthread_cond_signal(&printed_cond);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t t1, t2, t3, t4, t5;
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&suma_cond, NULL);

    pthread_create(&t1, NULL, writeThread, (void*)'1');
    pthread_create(&t2, NULL, writeThread, (void*)'2');
    pthread_create(&t3, NULL, writeThread, (void*)'3');
    pthread_create(&t4, NULL, writeThread, (void*)'4');
    pthread_create(&t5, NULL, printThread, NULL);

    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);

    return 0;

}