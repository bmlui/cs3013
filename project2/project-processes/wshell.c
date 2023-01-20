#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("wshell: command not found");
        return 1;
    }
    char* command = argv[1];
    if (strcmp(command, "exit") == 0) {
        return 0;
    }
    if (strcmp(command, "cd") == 0) {
        if (argc < 3) {
            printf("wshell: cd: missing argument");
            return 1;
        }
        char* path = argv[2];
        if (chdir(path) != 0) {
            printf("wshell: cd: cannot change directory to %s", path);
            return 1;
        }
        return 0;
    }
    if (strcmp(command, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s ", cwd);
        } else {
            printf("wshell: pwd: cannot get current working directory");
            return 1;
        }
        return 0;
    }
    if (strcmp(command, "help") == 0) {
        printf("wshell: help: no help available");
        return 0;
    }
    printf("wshell: command not found");
    return 1;
}