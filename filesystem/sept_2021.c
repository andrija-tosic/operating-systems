#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>

void processdir(char* folder, int depth, int max_depth) {
    DIR* dp;
    struct dirent* dirp;

    struct stat statbuf;
    int result;

    char path[255];

    if ((dp = opendir(folder)) == NULL) {
        exit(-1);
    }

    while ((dirp = readdir(dp)) != NULL) {

        strcpy(path, folder);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        if ((result = stat(path, &statbuf)) == -1) {
            continue;
        }

        if (S_ISREG(statbuf.st_mode)) {
            if (fork() == 0) {
                printf("path: %s\n", path);
                execlp("head", "head", path);
            }
            else {
                wait(NULL);
            }
        }

        if (max_depth == 0) {
            processdir(path, 0, max_depth);
        }

        if (depth < max_depth && strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
            processdir(path, depth + 1, max_depth);
        }
    }
}

int main(int argc, char* argv[]) {
    int max_dubina = atoi(argv[2]);
    processdir(argv[1], 0, max_dubina);
}