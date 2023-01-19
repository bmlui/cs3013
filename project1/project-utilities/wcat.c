#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
 if ( argc < 2)
    {
        return 0;
    }
    for (int i = 1; i<argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
        printf("wcat: cannot open file\n");
        return(1); }
            char s[10000] = "";
            do {
                printf("%s", s);
            }while(fgets(s, sizeof(s), fp));
        
        fclose(fp);
    }




}
