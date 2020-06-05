
/*
 * Import a module from another file.
 *
 * The node's name forms the symbol that the node is imported under. Access
 * to symbols defined in this node are accessed using the dot '.' operator with
 * the import symbol as the prefix.
 *
 * This uses the import path that has been specified on the command line, if any.
 * The file extention is added when the file becomes open for parsing. The file
 * extention is not a part of the progromatic symbol.
 *
 */
#include "common.h"
#include "internal.h"

/*
 * TODO: circular dependency: To detect a circular dependency, a tree must be made of the dependencies such that the
 * root of the tree can be said to "depend" on the nodes that appear under it. When a node depends on a node above
 * it, that is a circular dependency. The tree exists for the life of the compile. A binary tree can be formed by
 * adding a
 *
 * Lists? Each file that is included has a list of dependencies. If a name appears twice in the list, then that is
 * a circular dependency. The problem is when to start a new list. Do you start a separate list for every file
 * that is included? Or is a list made just of top level includes? How would you tell?
 */

//static fn_buf[256]; // max file name size on Linux filesystem

static int file_exists(char* fname) {

    FILE* fp;
    if((fp = fopen(fname, "r")) != NULL) {
        fclose(fp);
        return 1;
    }

    return 0;
}

static char* search_path(char* path, char* name) {

    char* buf = malloc(256);    // max path length under linux is 255
    char* raw = strdup(path);   // strtok destroys the string it parses

    for(char* p = strtok(raw, ":") ; p!= NULL; p = strtok(NULL, ":")) {
        strcpy(buf, p);
        strcat(buf, "/");
        strcat(buf, name);
        if(file_exists(buf)) {
            free(raw);
            return buf; // caller must free this
        }
    }

    free(raw);
    free(buf);
    return NULL;
}

static char* find_import_file(const char* base) {

    char* name;
    char* path;
    char* tmp = NULL;

    //memset(fn_buf, 0, sizeof(fn_buf));

    name = malloc(256);

    // add the file extention if it's not present
    strncpy(name, base, 252);
    tmp = strrchr(name, '.');
    if(tmp != NULL) {
        if(strcmp(tmp, ".s"))
            strcat(name, ".s");
    }
    else
        strcat(name, ".s");

    // first search in the command line
    if((tmp = GET_CONFIG_STR("FPATH")) != NULL) {
        path = strdup(tmp);
        tmp = search_path(path, name);
        free(path);
        if(tmp == NULL) {
            if((tmp = getenv("SIMP_INCLUDE")) != NULL) {
                path = strdup(tmp);
                tmp = search_path(path, name);
                free(path);
            }
        }
    }

    free(name);
    return tmp; // caller must free this
}

void parse_import(ast_node_t* node) {

    TRACE();
    scanner_state_t ss;
    int tok = get_token(&ss);

    if(tok == STRING_LITERAL) {
        char* fn = find_import_file(ss.value.str);
        if(fn != NULL) {
            parse_module(fn, node);
            free(fn);
        }
        else
            syntax("cannot find module \"%s\" to open", ss.value.str);
    }
    else
        syntax("expected a module name in quotes, but got %s", tok_to_strg(tok));

    expect_token(&ss, ';');

}
