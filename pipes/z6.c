/*
Napisati program na C-u koji simulira problem proizvodjac/potrosac koriscenjem datavoda.
Glavni program se deli u dva procesa. Prvi proces (proizvodjac) kreira N slucajnih pozitivnih 
celih brojeva i salje je ih drugom procesu. N se odredjuje tokom izvrsenja, takodje kao 
slucajan pozitivan ceo broj. Po zavrsetku slanja, prvi proces salje -1 kao kod za kraj.
Drugi proces (potrosac) preuzima poslate brojeve iz datavoda i stampa ih na standardnom izlazu.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int N = rand() % 100;
	int pd[2];

	if (pipe(pd) == -1)
	{
		exit(1);
	}

	if (fork() != 0)
	{
		close(pd[0]);

		int br;

		for(int i = 0; i < N; i++)
		{
			br = rand() % 100;
			write(pd[1], &br, sizeof(int));
		}
		br = -1;
		write(pd[1], &br, sizeof(int));

		close(pd[1]);
		wait(NULL);
	}
	else
	{
		close(pd[1]);

		int br;

		read(pd[0], &br, sizeof(int));

		while(br != -1)
		{
			printf("%d ", br);
			read(pd[0], &br, sizeof(int));
		}
		printf("\n");

		close(pd[0]);
	}
}