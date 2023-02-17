#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

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


int main(int argc, char* argv[]) {
  char* filename = argv[1];
  int n = atoi(argv[2]);

  if (n < 2 )
    exit(EXIT_FAILURE);

  int *input= malloc(sizeof(int) * n);

  read_input_vector(filename, n, input);

  for (int i=1; i < n; i++) {
    input[i] += input[i-1]; 
  }
  
  for (int i=0; i < n; i++) {
    printf("%d\n", input[i]); 
  }    
}
