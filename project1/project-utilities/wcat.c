
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	FILE *fp = fopen("foo.txt", "r"); 
if (fp == NULL) { 
puts("cannot open file\n");
exit(1); }
}
