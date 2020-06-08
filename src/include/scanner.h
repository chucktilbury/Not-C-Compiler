/*
 *  External interface to the parts of the scanner that get called from
 *  application code.
 */
#ifndef _SCANNER_H_
#define _SCANNER_H_

/*
 * Defined by flex. Call one time to isolate a symbol and then use the global
 * symbol struct to access the symbol.
 */
//extern int yylex(void);
//extern int yyparse(void);
//extern FILE* yyin;

// void yyerror(char *s, ...);
// void yyerror(const char* s);

typedef struct _scanner_state {
    int token;
    union __attribute__((packed)) {
        int64_t inum;
        uint64_t unum;
        double fnum;
        char str[1024];
    } value;
} scanner_state_t;

#define FIRST_TOKEN 256

typedef enum {
    ERROR_TOKEN,
    IMPORT = FIRST_TOKEN,
    EXTERN,
    CONST,
    STATIC,
    TYPEDEF,
    BREAK,

    CONTINUE,
    RETURN,
    YIELD,

    SWITCH,
    CASE,
    DEFAULT,
    DO,
    WHILE,
    FOR,
    IF,
    ELSE,
    MAIN,

    FLOAT,
    INT,
    UINT,
    BOOL,
    VOID,
    STRING,
    TUPLE,
    STRUCT,

    TRUE,
    FALSE,
    SIZEOF,
    TYPEOF,

    ELLIPSIS,

    AND_OP,
    OR_OP,
    LE_OP,
    GE_OP,
    EQ_OP,
    NE_OP,
    RIGHT_OP,
    LEFT_OP,

    STRING_LITERAL,
    INUM_LITERAL,
    FNUM_LITERAL,
    UNUM_LITERAL,
    IDENTIFIER,

    END_OF_INPUT,
    END_OF_FILE,

} token_t ;

char* get_file_name(void);
int get_line_number(void);
int get_col_number(void);
void open_file(const char* fname);
int get_token(scanner_state_t* ss);
const char* tok_to_strg(int tok);

#endif /* _SCANNER_H_ */
