#include <stddef.h>
#include "goatmalloc.h" // This include should appear last



void main() {
    goatmalloc_init();
    void *ptr = goatmalloc(100);
    goatfree(ptr);
    goatmalloc_stats();
}

