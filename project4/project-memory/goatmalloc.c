#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stddef.h>
#include "goatmalloc.h"

node_t *_arena_start;

int init(size_t size)
{
    int fd = open("/dev/zero", O_RDWR); 
    if (fd < 0)
    {
        perror("Error opening /dev/zero");
        return ERR_CALL_FAILED;
    }
    if (size <= 0 || size >= MAX_ARENA_SIZE) // Check for invalid size, account for unsinged
    {
        return ERR_BAD_ARGUMENTS; 
    }
    int page_size = getpagesize();
    size = (size + page_size - 1) / page_size * page_size; // Round up to page size

    printf("Initializing arena:\n");
    printf("...requested size %ld bytes\n", size);
    printf("...pagesize is %d bytes\n", page_size);
    printf("...adjusting size with page boundaries\n");
    printf("...adjusted size is %ld bytes\n", size);
    printf("...mapping arena with mmap()\n");

    _arena_start = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); // Map memory
    if (_arena_start == MAP_FAILED)                                              // Check for error
    {
        return ERR_SYSCALL_FAILED; // Return error code
    }
    printf("...arena starts at %p\n...arena ends at %p\n", _arena_start, (char *)_arena_start + size);
    printf("...initializing header for initial free chunk\n...header size is 32 bytes\n"); // Initialize header
    _arena_start->size = size - 32;
    _arena_start->is_free = 1;
    _arena_start->fwd = NULL;
    _arena_start->bwd = NULL;

    return size;
}

int destroy()
{
    if (_arena_start == NULL) // Check for uninitialized arena
    {
        return ERR_UNINITIALIZED;
    }
    int result = munmap(_arena_start, 4096);
    if (result != 0) // check for error
    {
        return ERR_SYSCALL_FAILED;
    }
    _arena_start = NULL; // set arena to NULL
    printf("Destroying Arena:\n...unmapping arena with munmap()\n");
    return result;
}

int statusno;
void *walloc(size_t size) // Allocate memory
{
    if (_arena_start == NULL) // Check for uninitialized arena
    {
        statusno = ERR_UNINITIALIZED;
        return NULL;
    }
    if (size <= 0 || size >= MAX_ARENA_SIZE) // Check for invalid size, account for unsinged
    {
        statusno = ERR_BAD_ARGUMENTS; // Return error code
        return NULL;
    }
    node_t *current = _arena_start;
    while (current != NULL)
    {
        if (current->is_free == 1 && current->size >= size)
        {
            current->is_free = 0;
            return current + 1;
        }
        current = current->fwd;
    }
    statusno = ERR_OUT_OF_MEMORY;
    return NULL;
}

void wfree(void *ptr) // Free memory
{
    if (_arena_start == NULL) // Check for uninitialized arena
    {
        statusno = ERR_UNINITIALIZED;
        return;
    }
    if (ptr == NULL) // Check for invalid pointer
    {
        statusno = ERR_BAD_ARGUMENTS;
        return;
    }
    node_t *current = _arena_start;
    while (current != NULL)
    {
        if (current + 1 == ptr)
        {
            current->is_free = 1;
            return;
        }
        current = current->fwd;
    }
    statusno = ERR_BAD_ARGUMENTS;
    return;
}