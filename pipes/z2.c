/*
Korišćenjem programskog jezika C kreirati dva Linux procesa koja komuniciraju korišćenjem 
datavoda. Glavni proces iz izvorišne datoteke čita 100 karaktera i korišćenjem datavoda 
pročitane karaktere prosleđuje drugom procesu koji ih upisuje u odredišnu datoteku čije se 
ime prosleđuje kao drugi argument komandne linije.  Ova procedura se ponavlja dok se ne 
iskopira kompletna datoteka.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Nedovoljno argumenata.\n");
		exit(1);
	}

	int pd[2];

	if (pipe(pd) == -1)
	{
		printf("Greska pri kreiranju datavoda.\n");
		exit(1);
	}

	if (fork() != 0)
	{
		close(pd[0]);

		FILE* f = fopen(argv[1], "r");

		if (f == NULL)
		{
			printf("Greska pri otvaranju fajla.\n");
			exit(1);
		}

		char c;
		int i = 0;

		while((c = fgetc(f)) != EOF && i < 100)
		{
			write(pd[1], &c, sizeof(char));
			i++;
		}

		fclose(f);
		close(pd[1]);

		wait(NULL);
	}
	else
	{
		close(pd[1]);

		FILE* f = fopen(argv[2], "w");

		if (f == NULL)
		{
			printf("Greska pri otvaranju fajla.\n");
			exit(1);
		}

		char c;
		int p;

		while((p = read(pd[0], &c, sizeof(char))) > 0)
		{
			fputc(c, f);
		}

		fclose(f);
		close(pd[0]);
	}
}