#ifndef __AST_H__
#define __AST_H__

typedef enum {
    NAME_ATTR = 400,
    TYPE_NAME_ATTR,
    DATA_TYPE_ATTR,
    IS_POINTER_ATTR,
    IMPORT_NAME_ATTR,
} ast_attr_type_t;

typedef enum {
    NUM_ATTR = 500,
    STR_ATTR,
    STRUCT_ATTR,
} ast_attr_store_type_t;

typedef struct {
    ast_attr_type_t type;
    const char* str;
    ast_attr_store_type_t stype;
} ast_attr_map_t;

extern ast_attr_map_t ast_attr_map[]; // defined in ast.c

typedef enum {
    AST_NO_ERROR,
    AST_END_LIST,
    AST_ATTR_NOT_FOUND,
} ast_error_t;

typedef enum {
    NO_NODE_TYPE,
    ROOT_NODE,
    IMPORT_NODE,
    TYPEDEF_NODE,
    DATA_DEF_NODE,
    FUNC_DEF_NODE,
    FUNC_DEF_PARM_NODE,
    EXPRESSION_ASSIGN_NODE,
    FUNC_PARAM_NODE,
    FUNC_BODY_NODE,
} ast_node_types_t;

typedef struct _ast_node {
    int node_type;
    hash_table_t* attribs;
    data_list_t* members;
} ast_node_t;

#define ADD_INT_ATTRIB(node, name, val) do { \
        int v = val; \
        if(add_node_attrib(node, name, &v, sizeof(int)) != AST_NO_ERROR) \
            fatal_error("cannot add int item to attribute table"); \
    } while(0)
#define ADD_STR_ATTRIB(node, name, str) do { \
        if(add_node_attrib(node, name, str, strlen(str)+1) != AST_NO_ERROR) \
            fatal_error("cannot add string item to attribute table"); \
    }while(0)

ast_node_t* create_node(int type);
void destroy_node(ast_node_t*);
void destroy_ast(ast_node_t* root);

int add_node_attrib(ast_node_t*, int, void*, size_t);
int get_node_attrib(ast_node_t*, int, void*, size_t);

void add_ast_node(ast_node_t*, ast_node_t*);
ast_node_t* get_member_list(ast_node_t*);
void reset_member_list(ast_node_t*);
ast_node_t* next_member(ast_node_t*);
int get_node_type(ast_node_t* node);
const ast_attr_map_t* attr_type_map(int type);
const ast_attr_map_t* attr_name_map(const char* name);
size_t get_node_attr_size(ast_node_t* node, int type);

void dump_ast(ast_node_t* root, const char* file);

#endif
