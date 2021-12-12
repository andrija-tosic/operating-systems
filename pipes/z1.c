/*
Korišćenjem programskog jezika C kreirati tri Linux procesa koja komuniciraju korišćenjem 
datavoda.  Prvi proces generiše niz proizvoljnih celih brojeva (dužina niza je fiksna i iznosi 
15 karaktera).  Ukoliko je prvi generisani broj paran, generisani niz brojeva se korišćenjem 
datavoda šalje drugom procesu koji ih upisuje u datoteku PARNI.txt.  Ukoliko je prvi 
generisani broj neparan, generisani niz brojeva se korišćenjem datavoda šalje trećem procesu 
koji ih štampa na standardnom izlazu.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	srand(time(0));

	int pd1[2], pd2[2];

	if (pipe(pd1) == -1 || pipe(pd2) == -1)
	{
		printf("Greska pri kreiranju datavoda.\n");
		exit(1);
	}

	if (fork() != 0)
	{
		close(pd1[0]);
		close(pd2[0]);

		int br;

		for (int i = 0; i < 15; i++)
		{
			br = rand() % 100;

			if (br % 2 == 0)
			{
				write(pd1[1], &br, sizeof(int));
			}
			else
			{
				write(pd2[1], &br, sizeof(int));
			}
		}

		wait(NULL);

		close(pd1[1]);
		close(pd2[1]);
	}
	else if (fork() != 0)
	{
		close(pd1[1]);
		close(pd2[0]);
		close(pd2[1]);

		int br;
		FILE* f = fopen("PARNI.txt", "w");

		if (f == NULL)
		{
			printf("Greska pri otvaranju fajla.\n");
			exit(1);
		}

		int p = read(pd1[0], &br, sizeof(int));

		while(p > 0)
		{
			fprintf(f, "%d ", br);

			p = read(pd1[0], &br, sizeof(int));
		}

		wait(NULL);

		close(pd1[0]);
	}
	else
	{
		close(pd1[0]);
		close(pd1[1]);
		close(pd2[1]);

		int br;

		int p = read(pd2[0], &br, sizeof(int));

		while(p > 0)
		{
			printf("%d ", br);

			p = read(pd2[0], &br, sizeof(int));
		}

		close(pd2[0]);
	}
}