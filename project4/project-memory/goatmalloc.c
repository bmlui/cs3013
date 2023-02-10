#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stddef.h>
#include "goatmalloc.h" // This include should appear last

void *_arena_start;

int init(size_t size)
{
    int fd = open("/dev/zero", O_RDWR);
    if (fd < 0)
    {
        perror("Error opening /dev/zero");
        return ERR_CALL_FAILED;
    }
    if (size <= 0 || size > MAX_ARENA_SIZE) // Check for invalid size, account for unsinged  
    {
        return ERR_BAD_ARGUMENTS; // Return error code
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
    if (_arena_start == MAP_FAILED) // Check for error
    {
        return ERR_SYSCALL_FAILED; // Return error code
    }

    printf("...arena starts at %p\n...arena ends at %p\n", _arena_start, (char *)_arena_start + size);
    printf("...initializing header for initial free chunk\n...header size is 32 bytes\n");

    return size;
}

int destroy()
{
    if (_arena_start == NULL)
    {
        return ERR_UNINITIALIZED;
    }
    int result = munmap(_arena_start, 4096);
    if (result != 0)
    {
        return ERR_SYSCALL_FAILED;
    }
    printf("Destroying Arena:\n...unmapping arena with munmap()\n");
    return result;
}
