#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

// TODO: Add more fields to this struct
struct job
{
  int id;
  int arrival;
  int length;
  int done;
  int time;
  struct job *next;
};

/*** Globals ***/
int seed = 100;

// This is the start of our linked list of jobs, i.e., the job list
struct job *head = NULL;

/*** Globals End ***/

/*Function to append a new job to the list*/
void append(int id, int arrival, int length)
{
  // create a new struct and initialize it with the input data
  struct job *tmp = (struct job *)malloc(sizeof(struct job));

  // tmp->id = numofjobs++;
  tmp->id = id;
  tmp->length = length;
  tmp->arrival = arrival;
  tmp->done = 0;

  // the new job is the last job
  tmp->next = NULL;

  // Case: job is first to be added, linked list is empty
  if (head == NULL)
  {
    head = tmp;
    return;
  }

  struct job *prev = head;

  // Find end of list
  while (prev->next != NULL)
  {
    prev = prev->next;
  }

  // Add job to end of list
  prev->next = tmp;
  return;
}

/*Function to read in the workload file and create job list*/
void read_workload_file(char *filename)
{
  int id = 0;
  FILE *fp;
  size_t len = 0;
  ssize_t read;
  char *line = NULL,
       *arrival = NULL,
       *length = NULL;

  // struct job **head_ptr = malloc(sizeof(struct job*));

  if ((fp = fopen(filename, "r")) == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) > 1)
  {
    arrival = strtok(line, ",\n");
    length = strtok(NULL, ",\n");

    // Make sure neither arrival nor length are null.
    assert(arrival != NULL && length != NULL);

    append(id++, atoi(arrival), atoi(length));
  }

  fclose(fp);

  // Make sure we read in at least one job
  assert(id > 0);

  return;
}

// FIFO policy
void policy_FIFO(struct job *head, int slice_duration)
{
  printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", slice_duration, head->id, head->arrival, head->length);
  head->time = slice_duration;
  head->done = 1;
  slice_duration += head->length;
  if (head->next != NULL)
  {
    policy_FIFO(head->next, slice_duration);
  }
  return;
}

// SJF policy
void policy_SJF(struct job *head, int slice_duration)
{
  int listCounter = 0;
  struct job *tmp = head;
  while (1 == 1)
  {
    if (tmp->next == NULL)
    {
      listCounter++;
      break;
    }
    else
    {
      listCounter++;
      tmp = tmp->next;
    }
  }

  for (int j = 0; j < listCounter; j++)
  {
    struct job *tmp = head;
    struct job *shortest = malloc(sizeof(struct job));
    shortest->id = -1;
    shortest->arrival = INT_MAX;
    shortest->length = INT_MAX;
    shortest->done = 1;
    shortest->next = NULL;

    while (1 == 1)
    {
      if ((tmp->length < shortest->length) && (tmp->done == 0) && (tmp->arrival <= slice_duration))
      {
        shortest = tmp;
      }
      if ((tmp->next == NULL) && (shortest->done == 0))
      {
        break;
      }
      else if ((tmp->next == NULL) && (shortest->done == 1))
      {
        slice_duration++;
        tmp = head;
      }
      else
      {
        tmp = tmp->next;
      }
    }

    printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", slice_duration, shortest->id, shortest->arrival, shortest->length);
    shortest->time = slice_duration;
    slice_duration += shortest->length;
    shortest->done = 1;
  }
}

// RR Policy
void policy_RR(struct job *head, int slice_duration)
{
 printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", slice_duration, head->id, head->arrival, head->length);
  head->time = slice_duration;
  head->done = 1;
  slice_duration += head->length;
  if (head->next != NULL)
  {
    policy_FIFO(head->next, slice_duration);
  }
  return;
}

// Analysis functions
//  analysis recursive fucntion
void analyze_r(struct job *head, double *avgResponse, double *avgTurnaround, double *avgWait, int *counter)
{
  *avgResponse += head->time - head->arrival;
  *avgTurnaround += head->time + head->length - head->arrival;
  *avgWait += head->time - head->arrival;
  *counter += 1;
  printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n", head->id, head->time - head->arrival, head->time + head->length - head->arrival, head->time - head->arrival);
  if (head->next != NULL)
  {
    analyze_r(head->next, avgResponse, avgTurnaround, avgWait, counter);
  }
  return;
}

// analysis starter function
void analyze(struct job *head)
{
  double avgResponse = 0;
  double avgTurnaround = 0;
  double avgWait = 0;
  int counter = 0;
  analyze_r(head, &avgResponse, &avgTurnaround, &avgWait, &counter);
  if (counter != 0)
  {
    avgResponse = avgResponse / counter;
    avgTurnaround = avgTurnaround / counter;
    avgWait = avgWait / counter;
  }
  printf("Average -- Response: %.2f  Turnaround %.2f  Wait %.2f\n", avgResponse, avgTurnaround, avgWait);
}

int main(int argc, char **argv)
{

  if (argc < 4)
  {
    fprintf(stderr, "missing variables\n");
    fprintf(stderr, "usage: %s analysis-flag policy workload-file slice-duration\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int analysis = atoi(argv[1]);
  char *policy = argv[2],
       *workload = argv[3];
  int slice_duration = atoi(argv[4]);

  // Note: we use a global variable to point to
  // the start of a linked-list of jobs, i.e., the job list
  read_workload_file(workload);

  // FIFO policy
  if (strcmp(policy, "FIFO") == 0)
  {
    slice_duration = 0;
    printf("Execution trace with FIFO:\n");
    policy_FIFO(head, slice_duration);
    printf("End of execution with FIFO.\n");
    if (analysis)
    {
      printf("Begin analyzing FIFO:\n");
      analyze(head);
      printf("End analyzing FIFO.\n");
    }

    exit(EXIT_SUCCESS);
  }

  // SJF policy
  if (strcmp(policy, "SJF") == 0)
  {
    slice_duration = 0;
    printf("Execution trace with SJF:\n");
    policy_SJF(head, slice_duration);
    printf("End of execution with SJF.\n");
    if (analysis)
    {
      printf("Begin analyzing SJF:\n");
      analyze(head);
      printf("End analyzing SJF.\n");
    }

    exit(EXIT_SUCCESS);
  }

  // RR policy
  if (strcmp(policy, "RR") == 0)
  {
    printf("Execution trace with RR:\n");
    policy_RR(head, slice_duration);
    printf("End of execution with RR.\n");
    if (analysis)
    {
      printf("Begin analyzing RR:\n");
      analyze(head);
      printf("End analyzing RR.\n");
    }

    exit(EXIT_SUCCESS);
  }

  exit(EXIT_SUCCESS);
}
