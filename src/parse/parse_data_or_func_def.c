

#include "common.h"
#include "internal.h"

int parse_expression(ast_node_t* node) {
    // TODO move this to its own file
    return 0;
}

int parse_func_body(ast_node_t* node) {
    // add to own file
    return 0;
}

int parse_indirection(ast_node_t* node) {

    int count = 1;
    int finished = 0;
    int retv = 0;
    int tok;
    scanner_state_t ss;

    while(!finished) {
        tok = expect_token_list(&ss, 2, '*', IDENTIFIER);
        if(tok == '*')
            count ++;
        else if(tok == IDENTIFIER) {
            ADD_STR_ATTRIB(node, NAME_ATTR, ss.value.str);
            ADD_INT_ATTRIB(node, IS_POINTER_ATTR, count);
            finished ++;
        }
        else {
            retv ++;
            finished ++;
        }
    }

    return retv;
}

int parse_func_def_parm_list(ast_node_t* node) {

    int retv = 0;
    int tok;
    scanner_state_t ss;
    int finished = 0;
    ast_node_t* n;

    while(!finished) {
        tok = get_token(&ss);
        if(is_type(&ss)) {
            n = create_node(FUNC_PARAM_NODE);
            if(tok == IDENTIFIER)
                ADD_STR_ATTRIB(n, TYPE_NAME_ATTR, ss.value.str);
            ADD_INT_ATTRIB(n, DATA_TYPE_ATTR, tok);
            add_ast_node(node, n);
        }
        else {
            syntax("expected a type specifier but got %s", tok_to_strg(tok));
            retv ++;
            finished ++;
        }

        tok = expect_token_list(&ss, 2, IDENTIFIER, '*');
        if(tok == '*') {
            retv += parse_indirection(n);
        }
        else if(tok == IDENTIFIER) {
            ADD_STR_ATTRIB(n, NAME_ATTR, ss.value.str);
        }
        else {
            retv ++;
        }

        tok = expect_token_list(&ss, 2, ',', ')');
        if(tok == ')') {
            finished ++;
        }
        else if(tok == ERROR_TOKEN){
            retv ++;
        }

        if(retv)
            finished ++;
    }
    return retv;
}

/*
 * A type has been read and it needs to be seen if this is a data definition or
 * if it's a function definition.
 *
 * type name '='|';' = a data definition
 * type name '(' = a function definition
 */
int parse_data_or_func_def(ast_node_t* node) {

    scanner_state_t ss;
    int retv = 0;

    // expect a '*' or a name
    int tok = expect_token_list(&ss, 2, '*', IDENTIFIER);

    if(tok == '*') {
        retv += parse_indirection(node);
    }
    else if(tok == IDENTIFIER) {
        ADD_STR_ATTRIB(node, NAME_ATTR, ss.value.str);
        //add_symbol(ss.value.str, node);
    }
    else
        retv ++; // error

    if(!retv) {
        tok = expect_token_list(&ss, 3, '(', '=', ';');
        if(tok == '(') {
            // parse the parameter list
            node->node_type = FUNC_DEF_PARM_NODE;
            retv += parse_func_def_parm_list(node);

            // parse the function body
            ast_node_t* n = create_node(FUNC_BODY_NODE);
            retv += parse_func_body(n);
            add_ast_node(node, n);
        }
        else if(tok == '=') {
            node->node_type = EXPRESSION_ASSIGN_NODE;
            ast_node_t* n = create_node(EXPRESSION_ASSIGN_NODE);
            retv += parse_expression(n);
            add_ast_node(node, n);
        }
        else if(tok == ';') {
            node->node_type = DATA_DEF_NODE;
            return retv; // no more processing here
        }
        else
            return retv + 1; // error
    }

    return retv;
}
