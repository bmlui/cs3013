#include <stdio.h>
#include <stdlib.h>


float raiseIt(float base, int exponent) {
   int i;
   float sum = 1;
   for (i=0; i<exponent; i++) {
        sum *= base;
     }
    return sum;
}

int main(int argc, char* argv[]) {
  int n = argc-2;
    if (n < 0) {
        printf("Error. Enter in x, a_0, a_1, ... of the summation.\n");
        return 1;
    }
        float returnSum = 0;
        int x = atoi(argv[1]);
        int i;
     for (i=0; i<n; i++) {
        float now =  raiseIt(x, i) * atof(argv[i+2]);
        returnSum += now;
     }
    printf("%f\n", returnSum);
     return 0;
}
