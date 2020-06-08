
#include "common.h"
#include "internal.h"

// This is a guard to prevent recursively importing a file. If it goes over 256, then an
// error is reported and the compiler aborts compilation.
static int entered = 0;

/*
 * This function is called recursively when an import statement is encountered.
 */
void parse_module(const char* name, ast_node_t* node) {

    int tok;
    int finished = 0;
    scanner_state_t ss;
    int err_flag = 0;

    entered ++;
    if(entered > 256)
        fatal_error("import nesting greater than 256 levels is not allowed");

    open_file(name);

    while(!finished) {
        tok = get_token(&ss);
        if(tok == IMPORT) {
            err_flag = 0;
            ast_node_t* n = create_node(IMPORT_NODE);
            err_flag += parse_import(n);
            add_ast_node(node, n);
        }
        else if(tok == TYPEDEF) {
            err_flag = 0;
            ast_node_t* n = create_node(TYPEDEF_NODE);
            err_flag += parse_typedef(n);
            add_ast_node(node, n);
        }
        else if(is_type(&ss)) {
            err_flag = 0;
            ast_node_t* n = create_node(NO_NODE_TYPE);
            // if the type is an identifier....
            if(tok == IDENTIFIER) {
                ADD_STR_ATTRIB(n, TYPE_NAME_ATTR, ss.value.str);
            }
            ADD_INT_ATTRIB(n, DATA_TYPE_ATTR, tok);
            // node type is not known yet.
            err_flag += parse_data_or_func_def(n);
            add_ast_node(node, n);
        }
        else if(tok == END_OF_INPUT || tok == END_OF_FILE) {
            finished++;
        }
        else if(get_num_errors() > 20) {
            finished++;
            syntax("abort compile due to errors");
        }
        else {
            // keep getting tokens without printing errors until an acceptible
            // token is found.
            if(err_flag == 0)
                syntax("expected import, data definition, or function definition, but got %s (%d)", tok_to_strg(tok), tok);
            err_flag += 1;

        }
    }
    entered --;
}

/*
 * Main entry point for the parser. Returns a pointer to the AST for further processing.
 */
ast_node_t* parse(const char* name) {

    ast_node_t* node = create_node(ROOT_NODE);
    ADD_STR_ATTRIB(node, NAME_ATTR, "__root__");

    parse_module(name, node);

    if(get_num_errors() == 0)
        return node; // root node
    else {
        destroy_ast(node);
        return NULL; // no further processing if there are errors
    }
}


