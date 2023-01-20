#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {
 if ( argc < 2 ) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    int runNum = argc;
    if (argc == 2) {
         runNum = argc+1;
    }
    char* searchTerm = argv[1];
    
    for (int i = 2; i<runNum; i++) {
        FILE *fp;
        if (argc ==2 ) {
           fp = stdin;
        } else {
            fp = fopen(argv[i], "r");
        }
            if (fp == NULL) {
            printf("wgrep: cannot open file\n");
            return(1); }
                char s[10000] = "";
                do {
                    if ((strstr(s, searchTerm) != NULL) | (strcmp(searchTerm," ") == 0)) {
                        printf("%s", s);
                    }
                }while(fgets(s, sizeof(s), fp));
            
            fclose(fp);
    }
   
    
    



}
