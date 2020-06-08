#ifndef __MISC_H__
#define __MISC_H__

/*
 * This macro works around the strncat() problem where the size is the length of the added
 * string, not the length of the total string. This accepts the length of the total string
 * and truncates the string to the maximum length if the added string would go over the
 * specified limit.
 */
#define STRNCAT(dest, src, size) strncat((dest), (src), size-strlen(dest))

/*
 * Clip the path to the main file name.
 */
const char* clip_path(const char* path);

#endif
