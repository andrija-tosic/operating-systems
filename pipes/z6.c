/*
Napisati program na C-u koji simulira problem proizvodjac/potrosac koriscenjem datavoda.
Glavni program se deli u dva procesa. Prvi proces (proizvodjac) kreira N slucajnih pozitivnih 
celih brojeva i salje je ih drugom procesu. N se odredjuje tokom izvrsenja, takodje kao 
slucajan pozitivan ceo broj. Po zavrsetku slanja, prvi proces salje -1 kao kod za kraj.
Drugi proces (potrosac) preuzima poslate brojeve iz datavoda i stampa ih na standardnom izlazu.
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int N = rand() % 5 + 1;

    int pd[2];
    pipe(pd);

    if (fork() != 0) {
        close(pd[0]);
        int br;
        for (int i=0; i<N; i++) {
            br = rand() % 10 + 1;
            write(pd[1], &br, sizeof(int));
        }
        br = -1;
        write(pd[1], &br, sizeof(int));
        close(pd[1]);
    }
    else {
        close(pd[1]);
        int recieved_br = 0;

        read(pd[0], &recieved_br, sizeof(int));
        while (recieved_br != -1) {
            printf("%d ", recieved_br);

            read(pd[0], &recieved_br, sizeof(int));
        }
        puts("");

        close(pd[0]);
    }
    return 0;
}