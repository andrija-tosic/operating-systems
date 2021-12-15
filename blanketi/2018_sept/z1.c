/*
Koriscenjem programskog jezika C napisati Linux progam koji sadrizi bafer u koji se mogu
smestiti dve celobrojne vrednosti- Zasebna nit periodicno generise dva slucajna broja 
(u opsegu od 100 do 199) i upisuje ih u ovaj bafer. Kada se u baferu nadje novi par brojeva, 
glavna nit (main funkcija) treba da odredi da li su ta dva broja jednaka i adekvatnu poruku 
na standardnom izlazu. Ovaj postupak generisanja para brojeva i ispitivanje njihove 
jednakosti treba da se ponovi 10 puta.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_cond_t cond2;
bool upisani = false;

int buffer[2] = { 0, 1 };

void* upisi(void* arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex);

		while (upisani)
		{
			pthread_cond_wait(&cond2, &mutex);
		}

		// buffer[0] = rand() % 100 + 100;
		// buffer[1] = rand() % 100 + 100;
		buffer[0] = rand() % 5;
		buffer[1] = rand() % 5;

		printf("1. %d\n2. %d\n", buffer[0], buffer[1]);

		upisani = true;
		pthread_cond_signal(&cond);

		pthread_mutex_unlock(&mutex);

		// sleep(1);
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	pthread_t thread;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_cond_init(&cond2, NULL);

	pthread_create(&thread, NULL, upisi, NULL);

	for (int i = 0; i < 20; i++)
	{
		pthread_mutex_lock(&mutex);

		while (!upisani)
		{
			pthread_cond_wait(&cond, &mutex);
		}

		if (buffer[0] == buffer[1])
		{
			printf("ISTI: %d = %d\n", buffer[0], buffer[1]);
		}
		upisani = false;

		pthread_cond_signal(&cond2);

		pthread_mutex_unlock(&mutex);
	}

	pthread_join(thread, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}