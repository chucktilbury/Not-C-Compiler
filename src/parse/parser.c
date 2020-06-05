
#include "common.h"
#include "internal.h"


/*
 * This function is called recursively when an import statement is encountered.
 */
void parse_module(ast_node_t* node) {

    int tok;
    int finished = 0;
    scanner_state_t ss;
    int err_flag = 0;

    while(!finished) {
        tok = get_token(&ss);
        if(tok == IMPORT) {
            err_flag = 0;
            ast_node_t* n = create_node();
            ADD_INT_ATTRIB(n, "NODE_TYPE", IMPORT_NODE);
            parse_import(n);
            add_node_as_sibling(node, n);
        }
        else if(tok == TYPEDEF) {
            err_flag = 0;
            ast_node_t* n = create_node();
            ADD_INT_ATTRIB(n, "NODE_TYPE", TYPEDEF_NODE);
            parse_typedef(n);
            add_node_as_child(node, n);
        }
        else if(is_type(&ss)) {
            err_flag = 0;
            ast_node_t* n = create_node();
            ADD_INT_ATTRIB(n, "DATA_TYPE", tok);
            // node type is not known yet.
            parse_data_or_func_def(n);
            add_node_as_child(node, n);
        }
        else if(tok == END_OF_INPUT) {
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
                syntax("expected import, data definition, or function definition, but got %s", tok_to_strg(tok));
            err_flag = 1;

        }
    }
}

/*
 * Main entry point for the parser. Returns a pointer to the AST for further processing.
 */
ast_node_t* parse(void) {

    ast_node_t* node = create_node();
    ADD_INT_ATTRIB(node, "NODE_TYPE", ROOT_NODE);
    ADD_STR_ATTRIB(node, "NAME", "__root__");

    parse_module(node);

    if(get_num_errors() == 0)
        return node; // root node
    else {
        destroy_ast(node);
        return NULL; // no further processing if there are errors
    }
}


