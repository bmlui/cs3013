#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/types.h>

// The read_input_vector function is from the serial soliton as allowed per the project instructions
#define MAX_LINE_SIZE 256
void read_input_vector(const char *filename, int n, int *array)
{
    FILE *fp;
    char *line = malloc(MAX_LINE_SIZE + 1);
    size_t len = MAX_LINE_SIZE;
    ssize_t read;

    fp = strcmp(filename, "-") ? fopen(filename, "r") : stdin;

    assert(fp != NULL && line != NULL);

    int index = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        array[index] = atoi(line);
        index++;
    }

    free(line);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    // Check for correct number of arguments
    if (argc != 4)
    {
        printf("Usage: %s <filename> <number of lines in the file> <number of threads to use>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Set vars
    char *filename = argv[1];
    int lines = atoi(argv[2]);
    int threads = atoi(argv[3]);

    // Check for valid input
    if (lines < 2 || threads < 1)
    {
        exit(EXIT_FAILURE);
    }

    // Reading input vector from file
    int *input = malloc(sizeof(int) * lines);
    read_input_vector(filename, lines, input); // The read_input_vector function is from the serial soliton per the project instructions
    int *output = malloc(sizeof(int) * lines);


    // Create threads
    pthread_t *thread = malloc(sizeof(pthread_t) * threads);
    int *thread_id = malloc(sizeof(int) * threads);




}