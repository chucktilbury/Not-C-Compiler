
/*
 * Simple regression test for stacks.
 *
 * build as:
 * gcc -Wall -Wextra -g test_stacks.c -I../src/include -L../lib -lutils -lparser
 */

#include "common.h"

char* strs[] = {"foo",      "bar",     "baz",   "bacon",     "eggs",     "potatoes", "onions",    "knuckles", "are",
                "dragging", "hoops",   "of",    "chocolate", "almonds",  "with",     "sprinkles", "and",      "cyanide",
                "log",      "balls",   "eaten", "by",        "unicorns", "as",       "pink",      "stripes",  "given",
                "to",       "nuclear", "pound", "cake",      "candles",  "snards",   "snipes",    NULL};

int main(void)
{
    stack_t* stack = create_stack();
    char buffer[128];
    int type;

    printf("\npush a few items\n");
    for(int i = 0; strs[i] != NULL; i++)
    {
        push_stack(stack, strs[i], strlen(strs[i]) + 1, i);
        printf("add: %s type: %d\n", strs[i], i);
    }
    printf("\npeek an item\n");
    type = peek_stack(stack, buffer, sizeof(buffer));
    printf("buffer: %s type: %d\n", buffer, type);
    type = peek_stack(stack, buffer, sizeof(buffer));
    printf("buffer: %s type: %d\n", buffer, type);

    printf("\npop a few items\n");
    type = pop_stack(stack, buffer, sizeof(buffer));
    printf("buffer: %s type: %d\n", buffer, type);
    type = pop_stack(stack, buffer, sizeof(buffer));
    printf("buffer: %s type: %d\n", buffer, type);
    type = pop_stack(stack, buffer, sizeof(buffer));
    printf("buffer: %s type: %d\n", buffer, type);

    printf("\npeek an item\n");
    type = peek_stack(stack, buffer, sizeof(buffer));
    printf("buffer: %s type: %d\n", buffer, type);

    printf("\ndestroy the stack\n");
    destroy_stack(stack);
}
