/*
 * Simple allocaion routines that will be able to track how much memory is in
 * use and detect where a memory leak is located.
 */

/*
 * In the future, these function will track memory usage and that functionality will
 * be turned on or off by the program verbosity setting.
 */

#include "common.h"

/*
 * Allocate a memory buffer using calloc().
 */
void* allocate_memory(size_t size) {

    return calloc(1, size);
}

/*
 * Allocate memory for some number of data structures.
 */
void* allocate_data(size_t num, size_t size) {

    return calloc(num, size);
}

/*
 * Reallocate a memory buffer using realloc().
 */
void* reallocate_memory(void* ptr, size_t size) {

    return realloc(ptr, size);
}

/*
 * Allocate a string and copy the string to it.
 */
char* allocate_string(const char* str) {

    return strdup(str);
}

/*
 * Free a memory buffer.
 */
void free_memory(void* ptr) {

    free(ptr);
}
