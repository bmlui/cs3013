#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    bool isRunning = true;
    char *history[10];
    int historyIndex = 0;

    while (isRunning)
    {
        char workingPath[100];
        getcwd(workingPath, 100);
        char *basename = strrchr(workingPath, '/');
        basename++;
        char input[100];
        if (strcmp(basename, "") == 0)
        {
            basename = "/";
        }
        printf("%s$ ", basename);
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';
        if (!isatty(fileno(stdin)))
        {
            printf("%s\n", input);
            fflush(stdout);
        }
        history[historyIndex % 10] = strdup(input);
        historyIndex++;

        if (strncmp(input, "exit", 4) == 0)
        {
            isRunning = false;
        }
        else if (strncmp(input, "cd", 2) == 0)
        {
            char *val = input + 3;
            char *dirname = input + 3;
            if (strstr(val, " "))
            {
                printf("wshell: cd: too many arguments\n");
            }
            else
            {
                if (strcmp(val, "..") == 0)
                {
                    val = val;
                }
                else if ((strcmp(input, "cd") == 0) | (strcmp(input, "cd ") == 0))
                {
                    val = getenv("HOME");
                }
                else if (strncmp(val, "/", 1) != 0)
                {
                    dirname = val;
                    val = strcat(strcat(workingPath, "/"), val);
                }
                int rv = chdir(val);
                if (rv != 0)
                {
                    printf("wshell: no such directory: %s\n", dirname);
                }
            }
        }
        else if (strncmp(input, "echo", 4) == 0)
        {
            if (strncmp(input, "echo ", 5) == 0)
            {
                char *val = input + 5;
                printf("%s\n", val);
            }
            else
            {
                printf("\n");
            }
        }
        else if (strcmp(input, "pwd") == 0)
        {
            printf("%s\n", workingPath);
        }
        else if (strcmp(input, "help") == 0)
        {
            printf("wshell: a simple shell written in C");
        }
        else if (strcmp(input, "history") == 0)
        {
            if (historyIndex < 10)
            {
                for (int i = 0; i < historyIndex; i++)
                {
                    printf("%s\n", history[i]);
                }
            }
            else
            {
                for (int i = historyIndex; i < historyIndex + 10; i++)
                {
                    int a = i % 10;
                    printf("%s\n", history[a]);
                }
            }
        }

        else
        {
            //   printf("wshell: could not execute command: %s\n", input);
            int pid = fork();
            if (pid < 0)
            {
                printf("wshell: fork failed\n");
            }
            else if (pid == 0)
            {
                char *args[100];
                char *token = strtok(input, " ");
                int i = 0;
                while (token != NULL)
                {
                    args[i] = token;
                    token = strtok(NULL, " ");
                    i++;
                }
                args[i] = NULL;
                int rv = execvp(args[0], args);
                if (rv != 0)
                {
                    printf("wshell: could not execute command: %s\n", input);
                    exit(0);
                }
            }
            else
            {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }
}
