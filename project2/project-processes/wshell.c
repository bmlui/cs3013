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
         if (strcmp(basename, "") == 0) {
            basename = "/";
        }
        printf("%s$ ", basename);
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';
       if (!isatty(fileno(stdin))) { 
            printf("%s\n", input);
            fflush(stdout);
             }
        if (strncmp(input, "exit", 4) == 0)
        {
            isRunning = false;
        }
        else if (strncmp(input, "cd", 2) == 0)
        {
            char *val = input + 3;
            char *dirname = input +3;
             if (strstr(val, " ")) {
                    printf("wshell: cd: too many arguments\n");
                }
            else
            {
                 if (strcmp(val, "..") == 0) {
                    val = val;
                }  else if ((strcmp(input, "cd") == 0)| (strcmp(input, "cd ") == 0))
            {
                val = getenv("HOME");
               
            } else if (strncmp(val, "/", 1) != 0)
                {
                    dirname = val;
                    val = strcat(strcat(workingPath, "/"), val);
                }  
                int rv = chdir(val);
                if (rv != 0) {
                    printf("wshell: no such directory: %s\n", dirname);
                }
            }
        }
        else if (strncmp(input, "echo", 4) == 0)
        {
            if (strncmp(input, "echo ", 5)  == 0 ) {
            char * val = input +5; 
            printf("%s\n", val);
            } else {
                printf("\n");
            }
          
        }
          else if (strcmp(input, "pwd") == 0)
        {
            printf("%s\n",workingPath);
          
        }
        else
        {
            printf("wshell: could not execute command: %s\n", input);
        }
        
    }
}
