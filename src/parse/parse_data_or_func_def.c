

#include "common.h"
#include "internal.h"

/*
 * A type has been read and it needs to be seen if this is a data definition or
 * if it's a function definition.
 *
 * type name '='|';' = a data definition
 * type name '(' = a function definition
 */
void parse_data_or_func_def(ast_node_t* ps) {

    // expect a '*' or a name
}
