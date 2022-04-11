/*
Koriscenjem programskog jezika C napisati Linux program u kome pet niti pristupa deljivom
celobrojnom nizu. Cetiri nit (na 3 s) slucjno odabranom elementu niza dodaje slucajno
generisanu celobrojnu vrednost iz opsega [-10 : 10]. Poslednja nit stampa elemente niza 
samo ukoliko je njihova suma (nakon izmene od strane ostalih niti) paran broj. Za 
sinhronizaciju niti iskoristiti mehanizam mutex-a i uslovnih promenljivih.
*/

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 5

int niz[N] = { 1, 2, 3, 4, 5 };

pthread_mutex_t mutex;
pthread_cond_t cond_parna;
bool p = false;

void* generisi(void* arg)
{
	int sum;

	while (1)
	{
		pthread_mutex_lock(&mutex);

		p = false;

		sum = 0;

		int index = rand() % N;
		int broj = (rand() % 21) - 10;

		niz[index] = broj;

		for (int i = 0; i < N; i++)
		{
			sum += niz[i];
		}

		if (sum % 2 == 0)
		{
			p = true;
			pthread_cond_signal(&cond_parna);
		}

		pthread_mutex_unlock(&mutex);

		sleep(1); // pseudo 3
	}
}

void* stampaj(void* arg)
{
	int sum;

	while (1)
	{
		pthread_mutex_lock(&mutex);

		sum = 0;
		
		while (!p)
		{
			pthread_cond_wait(&cond_parna, &mutex);;
		}
		p = false;

		for (int i = 0; i < N; i++)
		{
			printf("%d ", niz[i]);
			sum += niz[i];
		}
		printf("\tsum = %d\n", sum);

		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t niti[5];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_parna, NULL);

	for (int i = 0; i < 4; i++)
	{
		pthread_create(&niti[i], NULL, generisi, NULL);
	}
	pthread_create(&niti[4], NULL, stampaj, NULL);

	for (int i = 0; i < 5; i++)
	{
		pthread_join(niti[i], NULL);
	}
}