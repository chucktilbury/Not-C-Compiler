
#include "common.h"

typedef struct {
    int tok;
    char* name;
}  tok_str;

static tok_str tok_list[] = {
    {IMPORT, "import keyword"},
    {EXTERN, "extern keyword"},
    {CONST, "const keyword"},
    {STATIC, "static keyword"},
    {TYPEDEF, "typedef keyword"},
    {BREAK, "break keyword"},
    {CONTINUE, "continue keyword"},
    {RETURN, "return keyword"},
    {YIELD, "yield keyword"},
    {SWITCH, "switch keyword"},
    {CASE, "case keyword"},
    {DEFAULT, "default keyword"},
    {DO, "do keyword"},
    {WHILE, "while keyword"},
    {FOR, "for keyword"},
    {IF, "if keyword"},
    {ELSE, "else keyword"},
    {FLOAT, "float type specifier"},
    {INT, "int type specifier"},
    {UINT, "uint type specifier"},
    {BOOL, "bool type specifier"},
    {VOID, "void type specifier"},
    {STRING, "string type specifier"},
    {TUPLE, "tuple type specifier"},
    {STRUCT, "struct keyword"},
    {TRUE, "true value"},
    {FALSE, "false value"},
    {SIZEOF, "sizeof operator"},
    {TYPEOF, "typeof operator"},
    {ELLIPSIS, "elipsis"},
    {AND_OP, "logical and operator"},
    {OR_OP, "logical or operator"},
    {LE_OP, "less or equal operator"},
    {GE_OP, "greater or equal operator"},
    {EQ_OP, "equals operator"},
    {NE_OP, "not equal operator"},
    {RIGHT_OP, "right shift operator"},
    {LEFT_OP, "left shift operator"},
    {STRING_LITERAL, "string literal"},
    {INUM_LITERAL, "signed number literal"},
    {FNUM_LITERAL, "float literal"},
    {UNUM_LITERAL, "unsigned number literal"},
    {IDENTIFIER, "identifier"},
    {MAIN, "main function"},
    {-1, NULL}
};

static char str[25];

const char* tok_to_strg(int tok) {

    if(tok < FIRST_TOKEN) {
        sprintf(str, "'%c' token", tok);
        return str;
    }
    else {
        for(int i = 0; tok_list[i].name != NULL; i++) {
            if(tok_list[i].tok == tok)
                return tok_list[i].name;
        }
    }

    return NULL;
}
