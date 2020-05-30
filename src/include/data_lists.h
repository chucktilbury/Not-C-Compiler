#ifndef __DATA_LISTS_H__
#define __DATA_LISTS_H__
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Structure for a managed array.
 */
typedef struct
{
    size_t nitems;     // number of items currently in the array
    size_t index;      // current index for iterating the list
    size_t capacity;   // capacity in items
    size_t item_size;  // size of each item
    uint8_t* buffer;   // raw buffer where the items are kept
} data_list_t;

void init_data_list(data_list_t* list, size_t size);
data_list_t* create_data_list(size_t size);
void destroy_data_list(data_list_t* array);
void append_data_list(data_list_t* array, void* item);
void* get_data_list_by_index(data_list_t* array, int index);
void* get_data_list_next(data_list_t* array);
void reset_data_list(data_list_t* array);


#endif
