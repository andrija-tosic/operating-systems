/*
Koriscenjem programskog jezika C kreirati Linux program koji oponasa program more. Ovaj program 
ocekuje dva argumenta komandne linije. Prvi argument je naziv programa koji treba da startuje 
i ciji izlaz treba da prikazuje ekran po ekran. Drugi argument N je broj linija koje prikazuje 
na ekranu posle cega prikazuje poruku MORE, ceka pritisak bilo kog tastera i nakon toga 
prikazuje narednih N linija itd.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// ne valja

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Nedovoljno argumenata.\n");
		exit(1);
	}

	int pd1[2];
	int N = atoi(argv[argc - 1]);

	if (pipe(pd1) == -1)
	{
		printf("Greska pri kreiranju datavoda.\n");
		exit(1);
	}

	if (fork() == 0)
	{
		dup2(pd1[1], 1);
		close(pd1[0]);
		close(pd1[1]);

		char* args[argc];

		for (int i = 1; i < argc - 1; i++)
		{
			args[i - 1] = argv[i];
		}
		args[argc - 2] = NULL;

		execvp(argv[1], args);

		//execlp(argv[1], argv[1], NULL);
		// execlp("ls", "ls", "-la", NULL);
	}
	else
	{
		close(pd1[1]);

		int p, i;
		char linija[80];

		while((p = read(pd1[0], linija, 20)) > 0)
		{
			i = 0;
			while (p > 0 && i < N / 2)
			{
				printf("%s\n", linija);
				// printf("%s\n", linija);
				p = read(pd1[0], linija, 20);
				i++;
			}
			printf("--MORE--");
			getchar();
		}

		close(pd1[0]);
	}
}

// int main(int argc, char* argv[])
// {
// 	if (argc < 3)
// 	{
// 		printf("Nedovoljno argumenata.\n");
// 		exit(1);
// 	}

// 	int pd[2];

// 	if (pipe(pd) == -1)
// 	{
// 		printf("Greska pri kreiranju datavoda.\n");
// 		exit(1);
// 	}

// 	if (fork() == 0)
// 	{
// 		dup2(pd[1], 1);
// 		close(pd[0]);
// 		close(pd[1]);

// 		char* args[argc];

// 		for (int i = 1; i < argc - 1; i++)
// 		{
// 			args[i - 1] = argv[i];
// 		}
// 		args[argc - 2] = NULL;

// 		execvp(argv[1], args);
// 	}
// 	else
// 	{
// 		dup2(pd[0], 0);
// 		close(pd[0]);
// 		close(pd[1]);

// 		char* arg = malloc(strlen(argv[argc - 1]) + 2);
// 		strcat(arg, "-");
// 		strcat(arg, argv[argc - 1]);

// 		execlp("more", "more", arg, NULL);
// 	}
// }