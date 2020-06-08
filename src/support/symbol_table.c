
/*
 * Symbol table.
 *
 * Uses a hash table to track symbol names. References to symbols are stored whole, in dot notation.
 * Definitions to symbols are stored in parts, without the dots. When looking up a reference to a
 * symbol the dots are taken apart.
 *
 * The symbol table stores a pointer to the AST node that defines the symbol. Things like scope and
 * type are stored there.
 */

#include "common.h"

symbol_table_t create_symbol_table(void) {
    return (symbol_table_t)create_hash_table();
}

void destroy_symbol_table(symbol_table_t table) {
    destroy_hash_table((hash_table_t*)table);
}

int add_symbol(symbol_table_t table, const char* name, ast_node_t* node) {

    int retv = insert_hash_table(table, name, node, sizeof(ast_node_t));

    switch(retv) {
        case HASH_NO_ERROR:
            retv = ST_NO_ERROR;
            break;
        case HASH_EXIST:
            retv = ST_SYMBOL_EXISTS; // not added
            break;
        default:
            retv = ST_ERROR;
    }
    return retv;
}

int peek_symbol(symbol_table_t table, const char* name) {

    int retv = find_hash_table(table, name, NULL, 0);

    switch(retv) {
        case HASH_NO_ERROR:
            retv = ST_NO_ERROR;
            break;
        case HASH_NOT_FOUND:
            retv = ST_NOT_FOUND;
            break;
        default:
            retv = ST_ERROR;
    }
    return retv;
}

ast_node_t* get_symbol_reference(symbol_table_t table, const char* name) {

    ast_node_t* node = MALLOC(sizeof(ast_node_t));
    int retv = find_hash_table(table, name, node, sizeof(ast_node_t));

    switch(retv) {
        case HASH_NO_ERROR:
            return node;
        default:
            FREE(node);
            return NULL;
    }
    return node; // caller must free this
}

