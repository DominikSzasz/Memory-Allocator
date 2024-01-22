#ifndef ALLOCATOR_H
#define ALLOCATOR_H
void initialize_allocator(size_t pool_size);
void deallocate_allocator();
void free_mem(void* ptr);
void* alloc_mem(size_t size);
#endif