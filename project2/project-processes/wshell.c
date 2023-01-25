#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    bool isRunning = true;

    while (isRunning)
    {
        char workingPath[100];
        getcwd(workingPath, 100);
        char *basename = strrchr(workingPath, '/');
        basename++;
        char input[100];
        printf("%s$ ", basename);
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';
        if (strncmp(input, "exit", 4) == 0)
        {
            isRunning = false;
        }
        else if (strncmp(input, "cd", 2) == 0)
        {
            char *val = input + 3;
            if ((strcmp(input, "cd") == 0)| (strcmp(input, "cd ") == 0))
            {
                val = getenv("HOME");
            }
            else
            {
                if (strcmp(val, "..") == 0) {
                    val = val;
                } else if (strncmp(val, "/", 1) != 0)
                {
                    val = strcat(strcat(workingPath, "/"), val);
                }  
                chdir(val);
            }
        }
        else if (strncmp(input, "echo", 4) == 0)
        {
            if (strncmp(input, "echo ", 5)  == 0 ) {
            char * val = input +5; 
            printf("%s\n", val);
            }
          
        }
        else
        {
            system(input);
            printf("Command not found\n");
        }
    }
}
