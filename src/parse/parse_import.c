
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

static int file_exists(char* fname) {

    FILE* fp;
    if((fp = fopen(fname, "r")) != NULL) {
        fclose(fp);
        return 1;
    }

    return 0;
}

static char* search_env_path(char* name) {

    char* tmp = getenv("SIMP_INCLUDE");

    if(tmp == NULL)
        return NULL;

    char* buf = MALLOC(256);    // max path length under linux is 255
    char* raw = STRDUP(tmp);   // strtok destroys the string it parses

    for(char* p = strtok(raw, ":") ; p!= NULL; p = strtok(NULL, ":")) {
        strcpy(buf, p);
        strcat(buf, "/");
        strcat(buf, name);
        if(file_exists(buf)) {
            FREE(raw);
            return buf; // caller must free this
        }
    }

    FREE(raw);
    FREE(buf);
    return NULL;
}

static char* search_cmd_path(char* name) {

    char* ptr;
    char* buf = MALLOC(256);

    reset_config_list("FPATH");
    for(ptr = iterate_config("FPATH"); ptr != NULL; ptr = iterate_config("FPATH")) {
        strcpy(buf, ptr);
        strcat(buf, "/");
        strcat(buf, name);
        if(file_exists(buf)) {
            return buf; // caller must free this
        }
    }

    FREE(buf);
    return NULL;
}

char* find_import_file(const char* base) {

    char* name;
    char* tmp = NULL;

    //memset(fn_buf, 0, sizeof(fn_buf));

    name = MALLOC(256);

    // add the file extention if it's not present
    strncpy(name, base, 252);
    tmp = strrchr(name, '.');
    if(tmp != NULL) {
        if(strcmp(tmp, ".s"))
            strcat(name, ".s");
        else
            warning("do not include the file extention for import names");
    }
    else
        strcat(name, ".s");

    if(NULL == (tmp = search_cmd_path(name))) {
        if(NULL == (tmp = search_env_path(name))) {
            tmp = NULL; // for illustration
        }
    }

    FREE(name);
    return tmp; // caller must free this
}

int parse_import(ast_node_t* node) {

    scanner_state_t ss;
    int tok = get_token(&ss);

    if(tok == STRING_LITERAL) {
        ADD_STR_ATTRIB(node, IMPORT_NAME_ATTR, ss.value.str);
        char* fn = find_import_file(ss.value.str);
        if(fn != NULL) {
            parse_module(fn, node);
            free(fn);
        }
        else {
            syntax("cannot find module \"%s\" to open", ss.value.str);
            return 1;
        }
    }
    else {
        syntax("expected a module name in quotes, but got %s", tok_to_strg(tok));
        return 1;
    }

    expect_token(&ss, ';');

    return 0;
}
