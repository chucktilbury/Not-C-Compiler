
%{
#include "common.h"

#define TOKSTR get_tok_str()
void yyerror(const char* s);
%}
    //%define parse.error verbose
%define parse.error custom
%locations
%debug
%defines

%token SYMBOL   // a name
%token UNUM     // a literal unsigned int
%token FNUM     // a literal floating point number
%token INUM     // a literal signed number
%token QSTRG    // a literl string
%token TRUE     // bool literal value
%token FALSE    // bool literal value
%token NOTHING
%token IMPORT
%token STRUCT
%token IF
%token ELSE
%token WHILE
%token DO
%token RETURN
%token YIELD
%token CHOOSE
%token ITEM
%token DEFAULT
%token BREAK
%token CONTINUE
%token AND
%token OR
%token NOT
%token EQ
%token NE
%token LE
%token GE
%token SHL
%token SHR
%token INC
%token DEC
%token GT
%token LT
%token ALLOCATE
%token DISPOSE
%token SIZEOF
%token TYPEOF
%token MAIN
%token STRING   // string keyword
%token BOOL     // bool keyword
%token UINT     // uint keyword
%token INT      // int keyword
%token FLOAT    // float keyword

%%

module
    : NOTHING
    ;

%%

extern char yytext[];

void yyerror(const char* s)
{
    fflush(stdout);
    fprintf(stderr, "\n%s: line %d: at %d: %s\n\n", get_file_name(), get_line_number(), get_col_number(), s); //yylloc.first_line, s);
    inc_error_count();
}

int yyreport_syntax_error(const yypcontext_t *ctx)
{
    int res = 0;

    fflush(stdout);
    fprintf(stderr, "Syntax error in %s, line %d.%d:", get_file_name(), get_line_number(), get_col_number());
    inc_error_count();

    // Report the tokens expected at this point.
    {
        enum { TOKENMAX = 5 };
        yysymbol_kind_t expected[TOKENMAX];
        int n = yypcontext_expected_tokens (ctx, expected, TOKENMAX);
        if (n < 0)
            // Forward errors to yyparse.
            res = n;
        else
            for (int i = 0; i < n; ++i)
            fprintf (stderr, "%s %s",
                     i == 0 ? " expected" : " or", yysymbol_name (expected[i]));
    }

    // Report the unexpected token.
    {
        yysymbol_kind_t lookahead = yypcontext_token (ctx);
        if (lookahead != YYSYMBOL_YYEMPTY)
            fprintf (stderr, " before %s", yysymbol_name (lookahead));
    }

    fprintf (stderr, "\n");
    return res;
}
