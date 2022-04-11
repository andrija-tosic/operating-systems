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

pthread_mutex_t mutex;

void* odbrojavaj(void* arg)
{
	pthread_mutex_lock(&mutex);
	int n = atoi((char*)arg);

	for (int i = n; i >= 0; i--)
	{
		printf("%d\n", i);
		sleep(1);
	}
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
	pthread_t thread;
	pthread_mutex_init(&mutex, NULL);

	char unos[20];

	scanf("%s", unos);
	while (strcmp(unos, "KRAJ") != 0)
	{
		pthread_create(&thread, NULL, odbrojavaj, (void*)unos);	
		pthread_join(thread, NULL);

		pthread_mutex_lock(&mutex);
		scanf("%s", unos);
		pthread_mutex_unlock(&mutex);
	}
}