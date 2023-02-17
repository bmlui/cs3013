#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
  int n = atoi(argv[1]);
  int mode = atoi(argv[2]);

  if (argc < 3 ) {
    printf("Usage: ./generator n mode\n");
  }

  srand ( time(NULL) );

  int number = 0;

  for(int i = 0; i < n; i++) {
    if (!mode) {
      number = 1;
    } else if (mode == 1) {
      number++; 
    } else if (mode == 2) {
      number = rand() % 1000+1;
    } else if (mode == 3) {
      number = (i % 2) ? -1 : 1; 
    }

    printf("%d\n", number);
  }

  return 0;
}
