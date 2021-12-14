/*
Koriscenjem programskog jezika C napisati Linux program u kome korisnik sa tastature unosi
recenicu. Glavni program zatim kreira dve niti, Prva nit sortira reci u unetoj recenici u 
leksikografski opadajucem redosledu, a po zavsetku sortiranja druga nit na ekranu stampa 
tu recenicu tako da samo prva rec pocinje velikim slovom, a sve ostale reci malim. Po 
zavrsetku rada ove dve niti glavni program treba da odstampa: "KRAJ".
*/

// ne radi

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

void* sortiraj(void* arg)
{
	char* recenica = (char*)arg;
	int len = strlen(recenica);
	char* tmp = malloc(len * sizeof(char) + 1);

	char** reci = malloc(10 * sizeof(char*)); // 10 reci max
	for (int i = 0; i < 10; i++)
	{
		reci[i] = malloc(len * (sizeof(char) + 1));
	}

	int brReci = 0;
	for (char* tok = strtok(recenica, " "); tok != NULL; tok = strtok(NULL, " "))
	{
		strcpy(reci[brReci], tok);
		printf("%s\n", tok);
		brReci++;
	}
	strcpy(reci[brReci], "end");

	for (int i = 0; i < brReci - 1; i++)
	{
		for (int j = i + 1; j < brReci; j++)
		{
			if (strcmp(reci[i], reci[j]) > 0)
			{
				strcpy(tmp, reci[i]);
				strcpy(reci[i], reci[j]);
				strcpy(reci[j], tmp);
			}
		}
	}

	free(tmp);

	return (void*)reci;
}

void* stampaj(void* arg)
{
	char** reci = (char**)arg;

	int i = 0;
	while (strcmp(reci[i], "end") != 0)
	{
		if (i == 0)
		{
			reci[i][0] = toupper(reci[i][0]);
		}
		else
		{
			reci[i][0] = tolower(reci[i][0]);
		}

		printf("%s ", reci[i]);

		i++;
	}

	fflush(stdout);
}

int main(int argc, char* argv[])
{
	char recenica[50];
	void* reci;
	pthread_t thread1, thread2;

	fgets(recenica, 50, stdin);

	pthread_create(&thread1, NULL, sortiraj, (void*)recenica);
	pthread_join(thread1, reci);

	pthread_create(&thread2, NULL, stampaj, reci);
	pthread_join(thread2, NULL);

	printf("KRAJ\n");

	// oslobodi mem
	// bolje s struct jer nemas duzinu od prvi nzi
}