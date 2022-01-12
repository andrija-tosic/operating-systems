/*
5.
Korišćenjem programskog jezika C napisati UNIX/Linux program koji
svom procesu detetu korišćenjem redova poruka identifikovan brojem 12010
prosleđuje karaktere unete sa tastature. Poruke su tipa 4.
Proces dobijene vrednosti upisuje u datoteku karakteri.txt.
Komunikacija se prekida kada korisnik unese bilo koji broj.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

struct msg {
    long tip;
    char karakter;
};

void main()
{
    int msqid;
    struct msg poruka;

    poruka.tip = 4;
    poruka.karakter = 'a';

    msqid = msgget(12010, 0666 | IPC_CREAT);

    if (fork() != 0) {
        while (!isdigit(poruka.karakter)) {
            printf("Unos karaktera: ");
            scanf("%c", &poruka.karakter);

            printf("Roditelj salje\n");
            msgsnd(msqid, &poruka, 1, 0);
        }

        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
        exit(0);
        
    } else {
        FILE* f = fopen("karakteri.txt", "w");
        
        while (!isdigit(poruka.karakter)) {
            printf("Proces dete prima\n");
            msgrcv(msqid, &poruka, 1, 4, 0);

            printf("Proces dete upisuje u fajl\n");

            fprintf(f, "%c", poruka.karakter);
        }

        fclose(f);
        exit(0);
    }
}