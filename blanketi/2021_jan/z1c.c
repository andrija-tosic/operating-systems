/*
Koriscenjem programskog jezika C napisati Linux program koji u glavnoj niti od korisnika 
ocekuje da unese ceo pozitivan broj. Zatim startuje drugu nit, prosledjuje joj uneti broj, 
a druga nit odbrojava (i stampa na ekranu brojeve kako odbrojava) od unetog broja do 0 sa 
pauzom od 2 sekunde izmedju dva uzastopna broja. Zatim u glavnoj niti korisnik ponovo unosi 
ceo broj i ovaj proces odbrojavanja i unosa broja se ponavija sve dok korisnik ne unese 
„KRAJ".
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem1, sem2;

void* odbrojavaj(void* arg)
{
	while (1)
	{
		sem_wait(&sem1);

		int n = atoi((char*)arg);
		for (int i = n; i >= 0; i--)
		{
			printf("%d\n", i);
			// sleep(2);
		}

		sem_post(&sem2);
	}
}

int main(int argc, char* argv[])
{
	pthread_t thread;
	sem_init(&sem1, 0, 1);
	sem_init(&sem2, 0, 0);

	char unos[20];
	
	scanf("%s", unos);

	pthread_create(&thread, NULL, odbrojavaj, (void*)unos);	

	while (strcmp(unos, "KRAJ") != 0)
	{
		sem_wait(&sem2);
		scanf("%s", unos);
		sem_post(&sem1);
	}

	pthread_join(thread, NULL);
	sem_destroy(&sem1);
	sem_destroy(&sem2);
}