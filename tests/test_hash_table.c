/*
 * Sanity check to make sure that the hash_table is
 * working.
 *
 * Build as:
 * gcc -Wall -Wextra -g test_hash_table.c -I../src/include -L../lib -lutils -lparser
 */

#include "common.h"

int main(void)
{
    char* strs[] = {"foo",      "bar",       "baz",      "bacon",   "eggs",    "potatoes",  "onions",
                    "knuckles", "are",       "dragging", "hoops",   "of",      "chocolate", "almonds",
                    "with",     "sprinkles", "and",      "cyanide", "log",     "balls",     "eaten",
                    "by",       "unicorns",  "as",       "pink",    "stripes", "given",     "to",
                    "nuclear",  "pound",     "cake",     "candles", "snards",  "and",       "snipes",    NULL};

    hash_table_t* tab = create_hash_table();
    char buffer[128];

    printf("\ninsert entries\n");
    printf("  table capacity: %lu\n", tab->capacity);
    printf("  table count: %lu\n", tab->count);
    for(int i = 0; strs[i] != NULL; i++)
        insert_hash_table(tab, strs[i], strs[i], strlen(strs[i]) + 1);

    printf("\nfind entries\n");
    printf("  table capacity: %lu\n", tab->capacity);
    printf("  table count: %lu\n", tab->count);

    for(int i = 0; strs[i] != NULL; i++)
    {
        find_hash_table(tab, strs[i], &buffer, sizeof(buffer));
        printf("%3d. value: %s\n", i+1, buffer);
    }

    strcpy(buffer, "glop");
    int re = find_hash_table(tab, "poopoo", &buffer, sizeof(buffer));
    printf("glop = %s, 2 = %d\n", buffer, re);

}


