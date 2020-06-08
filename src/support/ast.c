/**
 *
 * The abstract syntax tree is implemented as a tree that can have multiple
 * member nodes, which stored in a list. Members are children to the node and
 * they are siblings to each other.
 *
 * Note attributes are stored in a hash table by name.
 */
#include <time.h>
#include "common.h"

// this exists and is used to avoid storing stupid in the AST attributes.
ast_attr_map_t ast_attr_map[] = {
    {NAME_ATTR, "NAME", STR_ATTR},
    {TYPE_NAME_ATTR, "TYPE_NAME", STR_ATTR},
    {DATA_TYPE_ATTR, "DATA_TYPE", NUM_ATTR},
    {IS_POINTER_ATTR, "IS_POINTER", NUM_ATTR},
    {IMPORT_NAME_ATTR, "IMPORT_NAME", STR_ATTR},
    {-1, NULL, -1}
};

const ast_attr_map_t* attr_type_map(int type) {
    for(int i = 0; ast_attr_map[i].str != NULL; i++)
        if(ast_attr_map[i].type == type)
            return &ast_attr_map[i];

    fatal_error("cannot find ast_attr_map by type = %d", type); // does not return
    return NULL; // make the compiler happy.
}

const ast_attr_map_t* attr_name_map(const char* name) {
    for(int i = 0; ast_attr_map[i].str != NULL; i++)
        if(!strcmp(ast_attr_map[i].str, name))
            return &ast_attr_map[i];

    fatal_error("cannot find ast_attr_map by name = %s", name); // does not return
    return NULL; // make the compiler happy.
}


/*
 *  Create a node and return it. Memory must be free'd by destroy_node.
 */
ast_node_t* create_node(int type) {

    ast_node_t* node = MALLOC(sizeof(ast_node_t));
    if(node == NULL)
        fatal_error("cannot allocate %lu bytes for AST node", sizeof(ast_node_t));

    node->node_type = type;
    node->attribs = create_hash_table();
    node->members = create_data_list(sizeof(ast_node_t));
    return node;
}

/*
 * Destroy the underlying data structures. Note that this also frees the data structures
 * that the node contains.
 */
void destroy_node(ast_node_t* node) {

    destroy_hash_table(node->attribs);
    destroy_data_list(node->members);
}


/*
 * Recursively destroy the entire tree.
 */
void destroy_ast(ast_node_t* root) {
    // TBD
}

/*
 * Add a generic attribute. This function copies the data into an allocated memory
 * area. Care is required to prevent memory leaks when the stored data has allocated
 * pointers in it.
 */
int add_node_attrib(ast_node_t* node, int type, void* data, size_t size) {

    const char* name = attr_type_map(type)->str;
    int hv = insert_hash_table(node->attribs, name, data, size);

    switch(hv) {
        case HASH_NO_ERROR:
        case HASH_EXIST:
        case HASH_DATA_SIZE:
            return AST_NO_ERROR;
        case HASH_NOT_FOUND:
        case HASH_NO_DATA:
            return AST_ATTR_NOT_FOUND;
    }

    return AST_ATTR_NOT_FOUND;
}

/*
 * Retrieve an arbitrary data item from the hash table. The caller needs to know
 * what kind of data it is asking for. The data is copied out of the allocated
 * buffer in the hash table.
 */
int get_node_attrib(ast_node_t* node, int type, void* data, size_t size) {

    const char* name = attr_type_map(type)->str;
    int hv = find_hash_table(node->attribs, name, data, size);

    switch(hv) {
        case HASH_NO_ERROR:
        case HASH_EXIST:
        case HASH_DATA_SIZE:
            return AST_NO_ERROR;
        case HASH_NOT_FOUND:
        case HASH_NO_DATA:
            return AST_ATTR_NOT_FOUND;
    }

    return AST_ATTR_NOT_FOUND;
}

/*
 * Get the size of the attribute data.
 */
size_t get_node_attr_size(ast_node_t* node, int type) {

    const char* name = attr_type_map(type)->str;
    return find_hash_table_entry_size(node->attribs, name);
}

/*
 * Add a node link to the child list. This is typically done when the node has finished
 * parsing.
 */
void add_ast_node(ast_node_t* crnt, ast_node_t* node) {

    append_data_list(crnt->members, node);
}

/*
 * Reset the internal list index to zero.
 */
void reset_member_list(ast_node_t* node) {

    reset_data_list(node->members);
}

/*
 * Get the next child and increment the internal list pointer.
 */
ast_node_t* next_member(ast_node_t* node) {

    return (ast_node_t*)get_data_list_next(node->members);
}



