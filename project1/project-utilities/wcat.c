
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
 if ( argc < 2 )
    {
        printf( "Please enter a file name to open and print\n" );
        return 1;  
    }
    FILE *fp = fopen(argv[1], "r"); 

if (fp == NULL) { 
puts("Cannot open file\n");
exit(1); }
do{
} while(TRUE);

}
