/*
6.
Korišćenjem programskog jezika C napisati Linux program koji omogućava da dva procesa 
komuniciraju i sinhronizuju svoje izvršavanje korišćenjem signala. Prvi proces po slučajnom 
principu kreira 1024 celobrojne vrednosti i smešta ih u binarnu datoteku prenos.dat. Kada 
izgeneriše brojeve, obaveštava o tome drugi proces, slanjem signala SIGUSR1 i pauzira svoje 
izvršavanje. Kada drugi proces primi signal on sadržaj datoteke prenos.dat prikazuje na 
standardni izlazu, šalje prvom procesu signal SIGUSR2 i nakon toga pauzira svoje izvršavanje. 
Po prijemu signala SIGUSR2, prvi proces ponovo startuje čitav postupak. Postupak se ponavlja 
4096 puta. Nakon toga se oba procesa završavaju. 
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void sig1()
{
	printf("Signal 1 poslat\n");
}

void sig2()
{
	printf("Signal 2 poslat\n");
}

int main(int argc, char* argv)
{
	srand(time(NULL));
	signal(SIGUSR1, sig1);
	signal(SIGUSR2, sig2);

	int pid = fork();

	if (pid == 0)
	{
		FILE* file;
		int ppid = getppid();

		for (int i = 0; i < 4096; i++)
		{
			file = fopen("prenos.dat", "wb");

			for (int j = 0; j < 10; j++)
			{
				int br = rand();
				fwrite(&br, sizeof(int), 1, file);
			}

			fclose(file);

			kill(ppid, SIGUSR1);
			// printf("cekam\n");
			pause();
			// printf("primio\n");
		}
	}
	else
	{
		FILE* file;

		for (int i = 0; i < 4096; i++)
		{
			pause();
			file = fopen("prenos.dat", "rb");

			printf("%d. put\n", i + 1);
			for (int j = 0; j < 10; j++)
			{
				int br;
				fread(&br, sizeof(int), 1, file);
				printf("%d.\t %d\n", j + 1, br);
			}

			fclose(file);
			sleep(1);
			kill(pid, SIGUSR2);
		}

		wait(NULL);
	}
}