#ifndef __PARSER_H__
#define __PARSER_H__

ast_node_t* parse(const char* name);
void parse_module(const char* name, ast_node_t* node);

#endif
