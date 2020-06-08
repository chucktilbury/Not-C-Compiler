
#include "common.h"

/*
 * This function works around the strncat() problem where the size is the length of the added
 * string, not the length of the total string. This accepts the length of the total string
 * and truncates the string to the maximum length if the added string would go over the
 * specified limit.
 */
char* cat_string(char* dest, char* src, size_t size) {

    size_t s = strlen(dest);

    return strncat(dest, src, size-s);
}

const char* clip_path(const char* path) {

    char* tmp = strrchr(path, '/');
    if(tmp != NULL)
        return ++tmp;
    else
        return path;
}
