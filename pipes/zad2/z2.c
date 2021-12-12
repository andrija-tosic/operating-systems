/*
Korišćenjem programskog jezika C kreirati dva Linux procesa koja komuniciraju korišćenjem 
datavoda.  Glavni proces iz izvorišne datoteke čita 100 karaktera i korišćenjem datavoda 
pročitane karaktere prosleđuje drugom procesu koji ih upisuje u odredišnu datoteku čije se 
ime prosleđuje kao drugi argument komandne linije.  Ova procedura se ponavlja dok se ne 
iskopira kompletna datoteka.
*/
#include <unistd.h>
#include <stdio.h>


int main() {
    int pd[2];

    if (pipe(pd) < 0) {
        return -1;
    }

    char buf[100];

    char descriptor[20];
    sprintf(descriptor, "%d", pd[0]);

    if (fork() == 0) {
        close(pd[1]);
        execl("z2_process", "z2_process", "z2_dest.txt", descriptor, NULL);
    }
    else {
        FILE* f = fopen("z2_src.txt", "r");
        
        close(pd[0]);
        while (!feof(f)) {
            puts("p1 reading from src file");
            if (fread(buf, sizeof(char), 100, f))
               write(pd[1], buf, sizeof(buf));
        }
        
        close(pd[1]);
        fclose(f);
    }
    return 0;
}