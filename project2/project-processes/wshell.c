#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>


int main (int argc, char *argv[]) {
    bool isRunning = true;
    
   
while (isRunning)
{
    char workingPath[100];
     getcwd(workingPath, 100);
    char* basename = strrchr(workingPath, '/');
    basename++;
    char input[100];
    printf("%s$ ", basename );
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    if (strcmp(input, "exit") == 0)
    {
        isRunning = false;
    } else if (strncmp(input, "cd", 2) == 0)
    {
        printf("%s", strcat(strcat(workingPath, "/"), input));
        chdir(strcat(strcat(workingPath, "/"), input));
    } else if (strncmp(input, "echo", 4) == 0)
    {
        printf("%s", input);
    }
    else
    {
       printf("Command not found");
    }
}
 

}

