#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

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

struct thread_arg {
    int *input;
    int *output;
    int start;
    int end;
    size_t size;
    sem_t *sem;
};

void * compute(void * arg) {
    struct thread_arg *args = (struct thread_arg *)arg;
    int *input = args->input;
    int *output = args->output;
    int start = args->start;
    int end = args->end;
    size_t size = args->size;
    sem_t *sem = args->sem;
    for (int i = start; i < end; i++)
    {
        output[i] = input[i] + ((i == 0) ? 0 : output[i - 1]);
    }
    return NULL;
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

    // Check for invalid input
    if (lines < 2 || threads < 1)
    {
        exit(EXIT_FAILURE);
    }

    // Reading input vector from file
    int *input = malloc(sizeof(int) * lines);
    read_input_vector(filename, lines, input); // The read_input_vector function is from the serial soliton per the project instructions
    int *output = malloc(sizeof(int) * lines);
    // Initialize output
    for (int i = 0; i < lines; i++)
    {
        output[i] = 0;
    }

     // Create threads
    pthread_t thread[threads];
    struct thread_arg *thread_args = malloc(sizeof(struct thread_arg) * threads);
    int chunk_size = lines / threads;
    for (int i = 0; i < threads; i++)
    {
        int start = i * chunk_size;
        int end = (i == threads - 1) ? lines : (i + 1) * chunk_size;
        thread_args[i].input = input;
        thread_args[i].output = output;
        thread_args[i].start = start;
        thread_args[i].end = end;
        pthread_create(&thread[i], NULL, compute, (void *)&thread_args[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < threads; i++)
    {
        pthread_join(thread[i], NULL);
    }


// Print output
    for (int i = 0; i < lines; i++)
    {
        printf("%d\n", output[i]);

    }

        // Clean up
    free(input);
    free(output);
    free(thread_args);
   
}