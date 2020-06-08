/*
 * Simple allocaion routines that will be able to track how much memory is in
 * use and detect where a memory leak is located.
 */

/*
 * In the future, these function will track memory usage and that functionality will
 * be turned on or off by the program verbosity setting.
 */

#include "common.h"

void init_memory_system(void) {
    // TBD
}

void destroy_memory_system(void) {
    // TBD
}

/*
 * Allocate a memory buffer using calloc().
 */
#ifndef _DEBUGGING
void* allocate_memory(size_t size) {
#else
void* allocate_memory(const char* file, int line, const char* func, size_t size) {
#endif

    void* ptr = calloc(1, size);
    if(ptr == NULL)
        fatal_error("cannot allocate %lu bytes", size);

    TRACE("%s: %d: %s: MALLOC: %lu bytes to ptr %p", file, line, func, size, ptr);
    return ptr;
}

/*
 * Allocate memory for some number of data structures.
 */
#ifndef _DEBUGGING
void* allocate_data(size_t num, size_t size) {
#else
void* allocate_data(const char* file, int line, const char* func, size_t num, size_t size) {
#endif

    void* ptr = calloc(num, size);
    if(ptr == NULL)
        fatal_error("cannot allocate %lu bytes", size * num);

    TRACE("%s: %d: %s: CALLOC: %lu items of %lu bytes to ptr %p", file, line, func, num, size, ptr);
    return ptr;
}

/*
 * Reallocate a memory buffer using realloc().
 */
#ifndef _DEBUGGING
void* reallocate_memory(void* ptr, size_t size) {
#else
void* reallocate_memory(const char* file, int line, const char* func, void* ptr, size_t size) {
#endif

    void* nptr = realloc(ptr, size);
    if(nptr == NULL)
        fatal_error("cannot reallocate %lu bytes", size);

    TRACE("%s: %d: %s: REALLOC: %p ptr to %lu bytes to ptr %p", file, line, func, ptr, size, nptr);
    return nptr;
}

/*
 * Allocate a string and copy the string to it.
 */
#ifndef _DEBUGGING
char* allocate_string(const char* str) {
#else
char* allocate_string(const char* file, int line, const char* func, const char* str) {
#endif

    char* ptr = strdup(str);
    if(ptr == NULL)
        fatal_error("cannot allocate %lu bytes for string", strlen(str));

    TRACE("%s: %d: %s: STRDUP: %lu bytes to ptr %p", file, line, func, strlen(str)+1, ptr);
    return ptr;
}

/*
 * Free a memory buffer.
 */
#ifndef _DEBUGGING
void free_memory(void* ptr) {
#else
void free_memory(const char* file, int line, const char* func, void* ptr) {
    DEBUG("%s: %d: %s: FREE: %p", file, line, func, ptr);
#endif

    free(ptr);
}
