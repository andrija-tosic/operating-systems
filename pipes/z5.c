/*
Korišćenjem programskog jezika C kreirati Linux program koji kreira dodatni process sa kojim 
komunicira korišćenjem datavoda. Roditeljski process generiše 20 slučajnih brojeva u opsegu 
od 100 do 199 i šalje ih putem datavoda procesu detetu.  Proces dete na ekranu treba da 
odštampa samo brojeve koje dobije preko datavoda, a koji su deljivi sa 3. U slučaju broja 
koji nije deljiv sa 3 ne štampa ništa.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main() {
    int pd[2];
    if (pipe(pd) < 0)
        exit(-1);

    // roditelj
    if (fork() != 0) {
        close(pd[0]);
        srand(time(0));
        int broj;
        for (int i=0; i<20; i++) {
            broj = (rand() % 100) + 100;
            write(pd[1], &broj, sizeof(int)); 
        }
        close(pd[1]);
    }
    // dete
    else {
        close(pd[1]);
        int procitan;
        for (int i=0; i<20; i++) {
            read(pd[0], &procitan, sizeof(int));
            if (procitan % 3 == 0) {
                printf("Procitan %d, deljiv sa 3\n", procitan);
            }
        }
        close(pd[0]);
    }
    return 0;
}