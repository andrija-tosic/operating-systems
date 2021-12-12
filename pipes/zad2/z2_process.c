/*
Korišćenjem programskog jezika C kreirati dva Linux procesa koja komuniciraju korišćenjem 
datavoda.  Glavni proces iz izvorišne datoteke čita 100 karaktera i korišćenjem datavoda 
pročitane karaktere prosleđuje drugom procesu koji ih upisuje u odredišnu datoteku čije se 
ime prosleđuje kao drugi argument komandne linije.  Ova procedura se ponavlja dok se ne 
iskopira kompletna datoteka.
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    char* file_name = argv[1];

    char buf[100];

    int pd = atoi(argv[2]);

    FILE* f = fopen(file_name, "w");
    if (!f)
        puts("p2 file open error");
    else
        puts("p2 opened file");

    while(read(pd, buf, sizeof(buf))) {
        puts("p2 writing to file");
        if (!fwrite(buf, sizeof(char), 100, f)) {
            puts("nije upisano nista");
        }
    };

    close(pd);
    fclose(f);
    return 0;    
}