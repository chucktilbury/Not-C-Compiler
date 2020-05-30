

#include "common.h"
#include "internal.h"

/*
 * A type has been read and it needs to be seen if this is a data definition or
 * if it's a function definition.
 *
 * type name '='|';' = a data definition
 * type name '(' = a function definition
 */
void do_data_or_func_def(parser_state_t* ps) {

    // expect a '*' or a name
}
