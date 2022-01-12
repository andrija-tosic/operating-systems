#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pd[2];
    pipe(pd);

    int stdout_backup = dup(1);


    char* program = argv[1];
    char* file_name = argv[2];

    // roditelj cita iz datavoda i upisuje u fajl
    if (fork() != 0) {
        close(pd[1]);
        
        char c;

        FILE* f = fopen(file_name, "w");

        while (read(pd[0], &c, sizeof(char))) {
            if (c == 'b') {
                c = 'B';
            }
            fwrite(&c, sizeof(char), 1, f);
        }

        fclose(f);
        close(pd[0]);
        
        wait(NULL);
    }

    // dete pokrece program i upisuje u datavod
    else {
        dup2(pd[1], 1);
        close(pd[0]);
        close(pd[1]);
        execlp(program, program, NULL);
    }

    return 0;
}