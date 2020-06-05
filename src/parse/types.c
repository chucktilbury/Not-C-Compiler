
#include "common.h"
#include "internal.h"

/*
 * If the token and symbol have been defined as a type, return 1,
 * else return 0. Look the symbol up in the symbol table.
 */
int is_defined_type(scanner_state_t* ss) {

    return 0;
}

/*
 * If the token defined a type, then return 1, esle return 0
 */
int is_type(scanner_state_t* ss) {

    switch(ss->token) {
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
            if(is_defined_type(ss))
                return 1;
            else
                return 0;
    }
    return 0;
}

