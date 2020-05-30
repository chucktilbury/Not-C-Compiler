
#include "common.h"

/*
 * Simple test using an list of strings.
 * Build as:
 *
 * gcc -Wall -Wextra -g test_ptr_lists.c -I../src/include -L../lib -lutils -lparser
 */

int main(void)
{
    char* strs[] = {"foo", "barzippoblart", "baz", "bacon", "eggs", "potatoes", "onions", "nuclear", "powered", "chicken", NULL};
    ptr_list_t* list;
    char* ptr;

    list = create_ptr_list();
    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

    for(int i = 0; strs[i] != NULL; i++)
        append_ptr_list(list, strs[i]);

    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

    int i = 0;
    do {
        ptr = get_ptr_list_by_index(list, i++);
        printf("%d: value: %s\n", i, ptr);
    } while(ptr != NULL);

    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

    i = 1;
    reset_ptr_list(list);
    do {
        ptr = get_ptr_list_next(list);
        printf("%d: value: %s\n", i++, ptr);
    } while(ptr != NULL);

    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

}
