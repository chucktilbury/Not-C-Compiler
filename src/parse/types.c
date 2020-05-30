
#include "common.h"
#include "internal.h"

/*
 * If the token and symbol have been defined as a type, return 1,
 * else return 0. Look the symbol up in the symbol table.
 */
int is_defined_type(parser_state_t* ps) {

    return 0;
}

/*
 * If the token defined a type, then return 1, esle return 0
 */
int is_type(parser_state_t* ps) {

    switch(ps->ss->token) {
        case FLOAT:
        case INT:
        case UINT:
        case BOOL:
        case VOID:
        case STRING:
        case TUPLE:
        case STRUCT:
            return 1;
        case IDENTIFIER:
            if(is_defined_type(ps))
                return 1;
            else
                return 0;
    }
    return 0;
}

