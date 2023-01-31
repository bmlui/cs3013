#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int breakk = 0;
bool lastCmdFail = false;
void command(char *input, char *workingPath, char *history[10], int historyIndex)
{
    lastCmdFail = false;
    if (strncmp(input, "exit", 4) == 0)
    {
        breakk = 1;
    }
    else if (strncmp(input, "cd", 2) == 0)
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
            char* val = args[1];
        char *dirname = val;
        if (args[2] != NULL)
        {
            printf("wshell: cd: too many arguments\n");
            lastCmdFail = true;
        }
        else
        {
           
         if (args[1] == NULL)
            {
                val = getenv("HOME");
            }
            else if (strncmp(val, "/", 1) != 0)
            {
                val = strcat(strcat(workingPath, "/"), val);
            }
            int rv = chdir(val);
            if (rv != 0)
            {
                printf("wshell: no such directory: %s\n", dirname);
                lastCmdFail = true;
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
                lastCmdFail = true;
                exit(0);
            }
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) == 0)
            {
                lastCmdFail = false;
            }
            else
            {
                lastCmdFail = true;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    char *history[10];
    int historyIndex = 0;
    int rc = 0;
    while (true)
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

        // implements && and || operators
        if (strstr(input, "&&") != NULL)
        {
            char *token = strtok(input, "&&");
            char* token2 = strtok(NULL, "&&");
            command(token, workingPath, history, historyIndex);
             if (lastCmdFail == false)
                {
                    command(token2, workingPath, history, historyIndex);
                }
        }
        else if (strstr(input, "||") != NULL)
        {
            char *token = strtok(input, "||");
            char* token2 = strtok(NULL, "||");
             command(token, workingPath, history, historyIndex);
             if (lastCmdFail == true)
                {
                   command(token2, workingPath, history, historyIndex);
                }
        } else {
            command(input, workingPath, history, historyIndex);
            if (breakk == 1)
            {
                break;
            }
        }
    }
    return rc;
}
