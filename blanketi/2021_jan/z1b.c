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
#include <stdbool.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

bool uslov = false;

void* odbrojavaj(void* arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex);

		while (!uslov)
		{
			pthread_cond_wait(&cond, &mutex);
		}

		int n = atoi((char*)arg);
		for (int i = n; i >= 0; i--)
		{
			printf("%d\n", i);
			// sleep(2);
		}

		uslov = false;
		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char* argv[])
{
	pthread_t thread;
	pthread_mutex_init(&mutex, NULL);

	char unos[20];
	
	scanf("%s", unos);
	uslov = true;
	pthread_cond_signal(&cond);

	pthread_create(&thread, NULL, odbrojavaj, (void*)unos);	
	
	sleep(1);

	while (strcmp(unos, "KRAJ") != 0)
	{
		sleep(1);
		pthread_mutex_lock(&mutex);
		scanf("%s", unos);
		uslov = true;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}

	pthread_join(thread, NULL);
}