/*
Koriscenjem programskog jezika C napisati Linux program koji omogucava modifikaciju
standardnog izlaza bilo kog programa i snimanje standardnog izlaza u specificiranu datoteku. 
Naziv programa koji se startuje i ciji se izlaz modifikuje se zadaje kao prvi argument 
komandne linije, a naziv tekstualne datoteke u koju se upisuje izlaz kao drugi argument 
komandne linije. Standardni izlaz se modifikuje tako da se svako pojavljivanje karaktera 
"b" zamenjuje karakterom "B".
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Premalo argumenata.\n");
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
		dup2(pd[1], 1);
		close(pd[0]);
		close(pd[1]);

		execlp(argv[1], argv[1], NULL);
	}
	else
	{
		close(pd[1]);

		FILE* file = fopen(argv[2], "w");
		char c;

		while (read(pd[0], &c, sizeof(char)) > 0)
		{
			if (c == 'b')
			{
				c = 'B';
			}
			fputc(c, file);
		}

		fclose(file);
		close(pd[0]);
	}
}