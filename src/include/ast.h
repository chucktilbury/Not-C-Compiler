#ifndef __AST_H__
#define __AST_H__


typedef enum {
    AST_NO_ERROR,
    AST_END_LIST,
    AST_ATTR_NOT_FOUND,
} ast_error_t;

typedef enum {
    ROOT_NODE,
    IMPORT_NODE,
    TYPEDEF_NODE,
    DATA_DEF_NODE,
    FUNC_DEF_NODE,
} ast_node_types_t;

typedef struct _ast_node {
    hash_table_t* attribs;
    data_list_t* children;
    data_list_t* siblings;
} ast_node_t;

#define ADD_INT_ATTRIB(node, name, val) do { \
        int v = val; \
        if(add_attribute(node, name, &v, sizeof(int)) != AST_NO_ERROR) \
            fatal_error("cannot add int item to attribute table"); \
    } while(0)
#define ADD_STR_ATTRIB(node, name, str) do { \
        if(add_attribute(node, name, str, strlen(str)+1) != AST_NO_ERROR) \
            fatal_error("cannot add string item to attribute table"); \
    }while(0)

ast_node_t* create_node(void);
void destroy_node(ast_node_t*);
void destroy_ast(ast_node_t* root);

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
