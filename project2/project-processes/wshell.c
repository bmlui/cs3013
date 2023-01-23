#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/*char[]* run(char[] workingDir, char[] input)
{
    char[]* args = parse(input);
    if (strcmp(args[0], "cd") == 0)
    {
        chdir(args[1]);
        return workingDir;
    }
    else
    {
        int pid = fork();
        if (pid == 0)
        {
            execvp(args[0], args);
        }
        else
        {
            wait(NULL);
        }
    }
}*/

int main (int argc, char *argv[]) {
    bool isRunning = true;
    char workingPath[100];
    getcwd(workingPath, 100);
while (isRunning)
{
    char* basename = strrchr(workingPath, '/');
    basename ++;
    char input[100];
    printf("%s$ ", basename );
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    if (strcmp(input, "exit") == 0)
    {
        isRunning = false;
    }
    else
    {
        system(input);
    }
}
 

}

