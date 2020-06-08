#ifndef __INTERNAL_H__
#define __INTERNAL_H__

typedef struct _parser_state {
    ast_node_t* node;
    scanner_state_t* ss;
} parser_state_t;

// types.c
int is_type(scanner_state_t*);
int is_defined_type(scanner_state_t*);

int parse_import(ast_node_t*);
int parse_data_or_func_def(ast_node_t*);
int parse_typedef(ast_node_t*);

char* find_import_file(const char* base);
#endif
