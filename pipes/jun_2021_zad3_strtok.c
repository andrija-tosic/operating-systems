#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc < 3)
        exit(-1);

    int N = atoi(argv[3]);

    int pd[2];
    if (pipe(pd) < 0)
        exit(-1);

    int backup_stdout = dup(1);

    // redirect stdout to pipe
    dup2(pd[1], 1);

    if (fork() == 0) {
        close(pd[0]);
        close(pd[1]);
        execlp(argv[1], argv[1], argv[2], NULL);
    }
    else {
        close(pd[1]);

        char buf[500];

        wait(NULL);

        read(pd[0], buf, 500);
        close(pd[0]);

        // povratak stdout
        dup2(backup_stdout, 1);
        close(backup_stdout);

        char* tok = strtok(buf, "\n");
        int i = 0;
        while (tok != NULL) {
            if (i < N) {
                puts(tok);
                fflush(stdout);
                i++;
                tok = strtok(NULL, "\n");
            } else {
                i = 0;
                puts("----MORE----");
                fflush(stdout);
                getchar();
            }
        }
    }
    return 0;
}
