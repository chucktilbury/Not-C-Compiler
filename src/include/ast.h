#ifndef __AST_H__
#define __AST_H__


typedef enum {
    AST_NO_ERROR,
    AST_END_LIST,
    AST_ATTR_NOT_FOUND,
} ast_error_t;

typedef struct _ast_node {
    hash_table_t* attribs;
    data_list_t* children;
    data_list_t* siblings;
} ast_node_t;

ast_node_t* create_node(void);
void destroy_node(ast_node_t*);

int add_attribute(ast_node_t*, const char*, void*, size_t);
int get_attribute(ast_node_t*, const char*, void*, size_t);

void add_node_as_child(ast_node_t*, ast_node_t*);
ast_node_t* get_child_list(ast_node_t*);
void reset_child_list(ast_node_t*);
ast_node_t* next_child(ast_node_t*); //, ast_node_t*);

void add_node_as_sibling(ast_node_t*, ast_node_t*);
ast_node_t* get_sibling_list(ast_node_t*);
void reset_sibling_list(ast_node_t*);
ast_node_t* next_sibling(ast_node_t*); //, ast_node_t*);

void dump_ast(ast_node_t* root, const char* file);

#endif
