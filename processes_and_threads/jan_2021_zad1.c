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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t odbrojavaj_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t unos_cond = PTHREAD_COND_INITIALIZER;
int odbrojavaj_bool, unos_bool;

void* odbrojavaj(void* arg)
{
    pthread_mutex_lock(&mutex);
    while (1) {
        while (!odbrojavaj_bool) {
            pthread_cond_wait(&odbrojavaj_cond, &mutex);
        }
        puts("Thread ulaz");
        puts("Thread locked mutex");
        int n = atoi((char*)arg);
        for (int i = n; i >= 0; i--) {
            printf("%d\n", i);
        }
        odbrojavaj_bool = 0;
        unos_bool = 1;
        pthread_cond_signal(&unos_cond);
        puts("Thread izlaz");
    }
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
    pthread_t thread;

    char unos[20];

    puts("Unos:");
    scanf("%s", unos);
    odbrojavaj_bool = 1;
    pthread_cond_signal(&odbrojavaj_cond);

    pthread_create(&thread, NULL, odbrojavaj, (void*)unos);

    while (strcmp(unos, "KRAJ") != 0) {
        pthread_mutex_lock(&mutex);

        while (!unos_bool) {
            pthread_cond_wait(&unos_cond, &mutex);
        }

        puts("Unos:");
        scanf("%s", unos);
        odbrojavaj_bool = 1;
        unos_bool = 0;
        pthread_cond_signal(&odbrojavaj_cond);

        pthread_mutex_unlock(&mutex);
    }

    pthread_join(thread, NULL);
}