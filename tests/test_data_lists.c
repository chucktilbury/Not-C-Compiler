
#include "common.h"

/*
 * Simple test using an list of strings.
 * Build as:
 *
 * gcc -Wall -Wextra -g test_data_lists.c -I../src/include -L../lib -lutils -lparser
 */

int main(void)
{
    char* strs[] = {"foo", "barzippoblart", "baz", "bacon", "eggs", "potatoes", "onions", "nuclear", "powered", "chicken", NULL};
    data_list_t* list;
    char* ptr;

    list = create_data_list(30); // 30 character buffers
    printf("nitems: %lu\n", list->nitems);
    printf("item_size: %lu\n", list->item_size);
    printf("capacity: %lu\n", list->capacity);

    for(int i = 0; strs[i] != NULL; i++)
        append_data_list(list, strs[i]);

    printf("nitems: %lu\n", list->nitems);
    printf("item_size: %lu\n", list->item_size);
    printf("capacity: %lu\n", list->capacity);

    int i = 0;
    do {
        ptr = get_data_list_by_index(list, i++);
        printf("%d: value: %s\n", i, ptr);
    } while(ptr != NULL);

    printf("nitems: %lu\n", list->nitems);
    printf("item_size: %lu\n", list->item_size);
    printf("capacity: %lu\n", list->capacity);

    i = 1;
    reset_data_list(list);
    do {
        ptr = get_data_list_next(list);
        printf("%d: value: %s\n", i++, ptr);
    } while(ptr != NULL);

    printf("nitems: %lu\n", list->nitems);
    printf("item_size: %lu\n", list->item_size);
    printf("capacity: %lu\n", list->capacity);

    destroy_data_list(list);
}
