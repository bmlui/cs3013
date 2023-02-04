#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

// TODO: Add more fields to this struct
struct job
{
  int id;
  int arrival;
  int length;
  int done;
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
  // TODO: Fill this in
  printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", slice_duration, head->id, head->arrival, head->length);
  slice_duration += head->length;
  if (head->next != NULL)
  {
    policy_FIFO(head->next, slice_duration);
  }
  return;
}

void analyze_FIFO(struct job *head)
{
  // TODO: Fill this in

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
    struct job *shortest = head;
    while (1 == 1)
    {
      if (tmp->done == 0)
      {
        break;
      }
      else
      {
        tmp = tmp->next;
        shortest = tmp;
      }
    }
    while (1 == 1)
    {
      if ((tmp->length < shortest->length) && (tmp->done == 0))
      {
        shortest = tmp;
      }
      if (tmp->next == NULL)
      {
        break;
      }
      tmp = tmp->next;
    }
    printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", slice_duration, shortest->id, shortest->arrival, shortest->length);
    slice_duration += shortest->length;
    shortest->done = 1;
  }
}

void analyze_SJF(struct job *head)
{
  // TODO: Fill this in

  return;
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
    printf("Execution trace with FIFO:\n");
    policy_FIFO(head, slice_duration);
    printf("End of execution with FIFO.\n");
    if (analysis)
    {
      printf("Begin analyzing FIFO:\n");
      analyze_FIFO(head);
      printf("End analyzing FIFO.\n");
    }

    exit(EXIT_SUCCESS);
  }

  // SJF policy
  if (strcmp(policy, "SJF") == 0)
  {
    printf("Execution trace with SJF:\n");
    policy_SJF(head, slice_duration);
    printf("End of execution with SJF.\n");
    if (analysis)
    {
      printf("Begin analyzing SJF:\n");
      analyze_SJF(head);
      printf("End analyzing SJF.\n");
    }

    exit(EXIT_SUCCESS);
  }

  // TODO: Add other policies

  exit(EXIT_SUCCESS);
}
