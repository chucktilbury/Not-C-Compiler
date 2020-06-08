#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

enum {
    ST_NO_ERROR,
    ST_ERROR,
    ST_SYMBOL_EXISTS,
    ST_NOT_FOUND,
} symbol_table_error_t;

typedef hash_table_t* symbol_table_t;

symbol_table_t create_symbol_table(void);
void destroy_symbol_table(symbol_table_t table);

int add_symbol(symbol_table_t table, const char* name, ast_node_t* node);
int peek_symbol(symbol_table_t table, const char* name);
ast_node_t* get_symbol_reference(symbol_table_t table, const char* name);
ast_node_t* get_symbol_definition(symbol_table_t table, const char* name);

#endif
