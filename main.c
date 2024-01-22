#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
void* alloc_mem(int size)
{
    void* memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return memory;
}
void free_mem(void* ptr, int size)
{
    munmap(ptr, size);
}
int main()
{
    char* test = (char*)alloc_mem(100);
    strcpy(test, "hello");
    printf("%s", test);
    free_mem(test, 100);
    return 0;
}