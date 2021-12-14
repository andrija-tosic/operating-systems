/*
Koriscenjem programskog jezika C napisati Linux program koji u glavnoj niti od korisnika 
ocekuje da unese ceo pozitivan broj. Zatim startuje drugu nit, prosledjuje joj uneti broj, 
a druga nit odbrojava (i stampa na ekranu brojeve kako odbrojava) od unetog broja do 0 sa 
pauzom od 2 sekunde izmedju dva uzastopna broja. Zatim u glavnoj niti korisnik ponovo unosi 
ceo broj i ovaj proces odbrojavanja i unosa broja se ponavija sve dok korisnik ne unese 
„KRAJ".
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int signalCond;

void* odbrojavaj(void* arg)
{
    pthread_mutex_lock(&mutex);
    while (1) {
        while (!signalCond) {
            pthread_cond_wait(&cond, &mutex);
        }
        puts("Thread ulaz");
        puts("Thread locked mutex");
        int n = atoi((char*)arg);
        for (int i = n; i >= 0; i--) {
            printf("%d\n", i);
            // sleep(2);
        }
        signalCond = 0;
        puts("Thread izlaz");
    }
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
    pthread_t thread;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    char unos[20];

    puts("Unos:");
    scanf("%s", unos);
    signalCond = 1;
    pthread_cond_signal(&cond);

    pthread_create(&thread, NULL, odbrojavaj, (void*)unos);

    while (strcmp(unos, "KRAJ") != 0) {
        pthread_mutex_lock(&mutex);
        puts("Unos:");
        scanf("%s", unos);
        signalCond = 1;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
		sleep(3);
    }

    pthread_join(thread, NULL);
}