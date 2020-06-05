/**
 *
 * The abstract syntax tree is implemented as a tree that can have multiple
 * sibling and child nodes. A child node is one that is defined inside a "{}"
 * block and a sibling node is one that has the same scope as another.
 *
 * For example, if two functions are defined in a module, they are siblings.
 * If a function defines several variables, then they are a child to the
 * function but siblings to each other.
 *
 * Siblings and children are kept in an ordered list. A node's attributes are
 * kept in a hash table and are accessed by name.
 *
 * The AST node is simply a hash table and all of the attributes of the node,
 * including the child and sibling lists are attributes stored in the table.
 */
#include <time.h>
#include "common.h"

/*
 *  Create a node and return it. Memory must be free'd by destroy_node.
 */
ast_node_t* create_node(void) {

    ast_node_t* node = allocate_memory(sizeof(ast_node_t));
    if(node == NULL)
        fatal_error("cannot allocate %lu bytes for AST node", sizeof(ast_node_t));

    node->attribs = create_hash_table();
    node->children = create_data_list(sizeof(ast_node_t));
    node->siblings = create_data_list(sizeof(ast_node_t));
    return node;
}

/*
 * Destroy the underlying data structures. Note that this also frees the data structures
 * that the node contains.
 */
void destroy_node(ast_node_t* node) {

    destroy_hash_table(node->attribs);
    destroy_data_list(node->children);
    destroy_data_list(node->siblings);
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
int add_attribute(ast_node_t* node, const char* name, void* data, size_t size) {

    int hv = insert_hash_table(node->attribs, name, data, size);

    switch(hv) {
        case HASH_NO_ERROR:
        case HASH_REPLACE:
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
int get_attribute(ast_node_t* node, const char* name, void* data, size_t size) {

    int hv = find_hash_table(node->attribs, name, data, size);

    switch(hv) {
        case HASH_NO_ERROR:
        case HASH_REPLACE:
        case HASH_DATA_SIZE:
            return AST_NO_ERROR;
        case HASH_NOT_FOUND:
        case HASH_NO_DATA:
            return AST_ATTR_NOT_FOUND;
    }

    return AST_ATTR_NOT_FOUND;
}


/*
 * Add a node link to the child list. This is typically done when the node has finished
 * parsing.
 */
void add_node_as_child(ast_node_t* crnt, ast_node_t* node) {

    append_data_list(crnt->children, node);
}

/*
 * Reset the internal list index to zero.
 */
void reset_child_list(ast_node_t* node) {

    reset_data_list(node->children);
}

/*
 * Get the next child and increment the internal list pointer.
 */
ast_node_t* next_child(ast_node_t* node) {

    return (ast_node_t*)get_data_list_next(node->children);
}



/*
 * Add a node link to the sibling list. This is typically done when the node has finished
 * parsing.
 */
void add_node_as_sibling(ast_node_t* crnt, ast_node_t* node) {

    append_data_list(crnt->siblings, node);
}

/*
 * Reset the internal list index to zero.
 */
void reset_sibling_list(ast_node_t*node) {

    reset_data_list(node->siblings);
}

/*
 * Get the next sibling and increment the internal list pointer.
 */
ast_node_t* next_sibling(ast_node_t* node) { // , ast_node_t* sibl) {

    return get_data_list_next(node->siblings);
}


/*
 * Dump the AST as a .DOT file.
 */
static void get_node_attributes(FILE* fp, ast_node_t* node) {

    // TODO: create an API for this.
    _table_entry_t* raw = node->attribs->entries;
    size_t limit = node->attribs->capacity;

    for(size_t i = 0; i < limit; i++) {
        if(raw[i].key != NULL)
            //fprintf(fp, " %s = %p\\n", raw[i].key, raw[i].data);
            // TODO: display the values associated with the attribs
            fprintf(fp, " %s\\n", raw[i].key);
    }
}

static void dump_walk_ast(FILE* fp, ast_node_t* node, const char* prev_name, int c_or_s) {

    char name[30];
    ast_node_t* n;

    sprintf(name, "node_%p", node);
    fprintf(fp, "    %s [label=\"{attributes: \\n", name);
    get_node_attributes(fp, node);
    fprintf(fp, "|<f0>child|<f1>sibling}\"];\n");
    fprintf(fp, "    %s:%s -> %s;\n", prev_name, c_or_s? "<f0>" : "<f1>", name);
    fflush(fp);

    // do the children
    while(NULL != (n = next_child(node)))
        dump_walk_ast(fp, n, name, 1);

    // do the siblings
    while(NULL != (n = next_sibling(node)))
        dump_walk_ast(fp, n, name, 0);
}

/*
 * Dump the specified AST as a ".dot" file to the file name specified.
 * https://www.graphviz.org/doc/info/lang.html
 */
void dump_ast(ast_node_t* root, const char* file) {

    time_t t = time(NULL);
    FILE* outfile = fopen(file, "w");

    if(outfile == NULL) {
        fprintf(stderr, "Graph Error: Cannot open %s for writing: %s\n", file, strerror(errno));
        return;
    }

    fprintf(outfile, "// file name \"%s\" dumped %s", get_file_name(), ctime(&t));
    fprintf(outfile, "digraph Source_AST_Tree {\n");
    fprintf(outfile, "    label=\"file name %s dumped %s\"", get_file_name(), ctime(&t));
    fprintf(outfile, "    node [style=\"rounded,filled\" shape=record]\n\n");

    dump_walk_ast(outfile, root, "root", 1); // root only has children

    fprintf(outfile, "\n}\n");

    fclose(outfile);
}

