#ifndef __MEMORY_H__
#define __MEMORY_H__

typedef struct _memory_node {
    int magic;
    size_t size;
    struct _memory_node* prev;
    struct _memory_node* next;
    char *buffer;
} memory_node_t;

typedef struct {
    int nitems;
    size_t total_size;
    memory_node_t* first;
    memory_node_t* last;
} memory_system_t;

void init_memory_system(void);
void destroy_memory_system(void);
// void* allocate_memory(size_t size);
// void* allocate_data(size_t num, size_t size);
// void* reallocate_memory(void* ptr, size_t size);
// char* allocate_string(const char* str);
// void free_memory(void* ptr);

#ifndef _DEBUGGING
#  define MALLOC(s) allocate_memory(s)
#  define CALLOC(n, s) allocate_data(n, s)
#  define REALLOC(p, s) reallocate_memory(p, s)
#  define STRDUP(s) allocate_string(s)
#  define FREE(p) free_memory(p)
void* allocate_memory(size_t size);
void* allocate_data(size_t num, size_t size);
void* reallocate_memory(void* ptr, size_t size);
char* allocate_string(const char* str);
void free_memory(void* ptr);

#else
#  define MALLOC(s) allocate_memory(clip_path(__FILE__), __LINE__, __func__, s)
#  define CALLOC(n, s) allocate_data(clip_path(__FILE__), __LINE__, __func__, n, s)
#  define REALLOC(p, s) reallocate_memory(clip_path(__FILE__), __LINE__, __func__, p, s)
#  define STRDUP(s) allocate_string(clip_path(__FILE__), __LINE__, __func__, s)
#  define FREE(p) free_memory(clip_path(__FILE__), __LINE__, __func__, p)
void* allocate_memory(const char* file, int line, const char* func, size_t size);
void* allocate_data(const char* file, int line, const char* func, size_t num, size_t size);
void* reallocate_memory(const char* file, int line, const char* func, void* ptr, size_t size);
char* allocate_string(const char* file, int line, const char* func, const char* str);
void free_memory(const char* file, int line, const char* func, void* ptr);
#endif

#endif
