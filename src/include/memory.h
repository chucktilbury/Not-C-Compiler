#ifndef __MEMORY_H__
#define __MEMORY_H__

void* allocate_memory(size_t size);
void* allocate_data(size_t num, size_t size);
void* reallocate_memory(void* ptr, size_t size);
char* allocate_string(const char* str);
void free_memory(void* ptr);

#endif
