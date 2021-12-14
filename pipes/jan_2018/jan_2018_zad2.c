#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int pd[2];
    pipe(pd);

    char *src = argv[1], *dest = argv[2];

    if (fork() != 0) {
        close(pd[0]);

        char dest_name[100];
        strcpy(dest_name, dest);

        //write(pd[1], dest_name, strlen(dest_name) + 1);

        write(pd[1], dest_name, 100*sizeof(char));

        FILE* src_file = fopen(src, "r");

        char buf[200];

        while (!feof(src_file)) {
            fread(buf, sizeof(char), 200, src_file);

            write(pd[1], buf, 200*sizeof(char));
        }
        close(pd[1]);
        fclose(src_file);
    }
    else {
        close(pd[1]);
        char dest_name[100];
        read(pd[0], dest_name, 100*sizeof(char));

        FILE* dest_file = fopen(dest_name, "w");

        char buf[200];

        while (read(pd[0], buf, 200*sizeof(char))) {
            fwrite(buf, sizeof(char), 200, dest_file);
        }

        close(pd[0]);
        fclose(dest_file);
    }

    return 0;
}