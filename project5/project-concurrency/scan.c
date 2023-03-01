#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

sem_t sem;

// The read_input_vector function is from the serial soliton as allowed per the project instructions
#define MAX_LINE_SIZE 256
void read_input_vector(const char* filename, int n, int* array)
{
  FILE *fp;
  char *line = malloc(MAX_LINE_SIZE+1);
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
    int myLinesCount;
    int startLineIndex;
    int endLineIndex;
    int pScanTimes;
    int numThreads;
};
pthread_mutex_t * countMutex;
int count = 0;
void * compute(void * arg) {
   struct thread_arg *t_arg = (struct thread_arg *) arg;
   int *input = t_arg->input;
    int *output = t_arg->output;
   int myLinesCount = t_arg->myLinesCount;
   int startLineIndex = t_arg->startLineIndex;
    int endLineIndex = t_arg->endLineIndex;
    int pScanTimes = t_arg->pScanTimes;
    int threads = t_arg->numThreads;

    for (int i = 0; i < pScanTimes; i++) {
        count = 0;
        for (int j = startLineIndex; j < endLineIndex; j++) {
            if (j >= (int)pow(2, i)) {
                output[j] = input[j-(int)pow(2, i)] + input[j];
            } else {
                output[j] = input[j];
            }
        }

        
            // Wait for all threads to complete this stage
        pthread_mutex_lock(countMutex);
        (count)++;
        if (count == threads) {
            pthread_mutex_unlock(countMutex);
            sem_post(&sem);
        } else {
            pthread_mutex_unlock(countMutex);
            sem_wait(&sem);
        }

        memcpy(input + startLineIndex, output + startLineIndex, sizeof(int) * myLinesCount);
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
    int numThreads = atoi(argv[3]);
    int threadLines = (int)ceil((double)lines / numThreads);

    // Check for invalid input
    if (lines < 2 || numThreads < 1)
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
    // Initialize threads
    sem_init(&sem, 0, numThreads);
    countMutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(countMutex, NULL);
    pthread_t threads[numThreads];
    struct thread_arg t_args[numThreads];

    for (int i = 0; i < numThreads; i++) {
    t_args[i].input = input;
    t_args[i].output = output;
    t_args[i].myLinesCount = threadLines;
    t_args[i].startLineIndex = i * threadLines;
    t_args[i].endLineIndex = (i + 1) * threadLines;
    t_args[i].pScanTimes = (int)floor(log2(lines));
    t_args[i].numThreads = numThreads;
    pthread_create(&threads[i], NULL, compute, (void *) &t_args[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem);
    pthread_mutex_destroy(countMutex);

    /* Non thread version
    int pScanTimes = (int)floor(log2(lines));
    for (int i = 0; i < pScanTimes; i++) {
        for (int j = (int)pow(2, i)-1; j < lines; j++) {
            if (j >= (int)pow(2, i)) {
                output[j] = input[j-(int)pow(2, i)] + input[j];
            } else {
                output[j] = input[j];
            }
        }
        memcpy(input, output, sizeof(int) * lines); // Copy partial scan to input, so that it can be used in the next iteration
    }*/

    // Print output
    for (int i = 0; i < lines; i++) {
        printf("%d\n", output[i]);
    }

    // Clean up
    free(input);
    free(output);
    free(countMutex);

    return 0;
}
