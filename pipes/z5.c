/*
Korišćenjem programskog jezika C kreirati Linux program koji kreira dodatni process sa kojim 
komunicira korišćenjem datavoda. Roditeljski process generiše 20 slučajnih brojeva u opsegu 
od 100 do 199 i šalje ih putem datavoda procesu detetu.  Proces dete na ekranu treba da 
odštampa samo brojeve koje dobije preko datavoda, a koji su deljivi sa 3. U slučaju broja 
koji nije deljiv sa 3 ne štampa ništa.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int pd[2];

	if (pipe(pd) == -1)
	{
		printf("Greska pri kreiranju datavoda.\n");
		exit(1);
	}

	if (fork() != 0)
	{
		srand(time(0));
		int broj;

		close(pd[0]);

		for (int i = 0; i < 20; i++)
		{
			broj = rand() % 100 + 100;
			write(pd[1], &broj, sizeof(int));
		}

		close(pd[1]);

		wait(NULL);
	}
	else
	{
		int br;

		close(pd[1]);

		for (int i = 0; i < 20; i++)
		{
			read(pd[0], &br, sizeof(int));

			if (br % 3 == 0)
			{
				printf("%d ", br);	
			}
		}
		printf("\n");

		close(pd[0]);
	}
}