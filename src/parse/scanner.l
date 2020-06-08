/*
 *
 * ANSI C grammar, Lex specification
 * In 1985, Jeff Lee published this Lex specification together with a Yacc grammar for the April 30, 1985 ANSI C draft.
 * Tom Stockfisch reposted both to net.sources in 1987; that original, as mentioned in the answer to question 17.25 of
 * the comp.lang.c FAQ, can be ftp'ed from ftp.uu.net, file usenet/net.sources/ansi.c.grammar.Z.
 *
 * I intend to keep this version as close to the current C Standard grammar as possible; please let me know if you
 * discover discrepancies.
 *
 * Jutta Degener, 1995
 * https://www.lysator.liu.se/c/ANSI-C-grammar-l.html
 */

D           [0-9]
L           [a-zA-Z_]
H           [a-fA-F0-9]
E           [Ee][+-]?{D}+
FS          (f|F|l|L)
IS          (u|U|l|L)*

%{

#include "common.h"
#include "internal.h"

// string buffer to get token from
//static char strbuf[1024*64];

#define SET_TOKEN_STATE(t) do{ \
            scanner_state.token = t; \
            strncpy(scanner_state.value.str, yytext, sizeof(scanner_state.value.str)); \
            return t; \
        }while(0)

#define SET_STRG_STATE() do{ \
            scanner_state.token = STRING_LITERAL; \
            strncpy(scanner_state.value.str, buffer, sizeof(scanner_state.value.str)); \
            BEGIN(INITIAL); \
            return STRING_LITERAL; \
        }while(0)

#define SET_IDENT_STATE() SET_TOKEN_STATE(check_type())

#define SET_UNUM_STATE() do{ \
            scanner_state.token = UNUM_LITERAL; \
            scanner_state.value.unum = strtol(yytext, NULL, 16); \
            return UNUM_LITERAL; \
        } while(0)

#define SET_INUM_STATE() do{ \
            scanner_state.token = INUM_LITERAL; \
            scanner_state.value.inum = strtol(yytext, NULL, 10); \
            return INUM_LITERAL; \
        } while(0)

#define SET_FNUM_STATE() do{ \
            scanner_state.token = FNUM_LITERAL; \
            scanner_state.value.fnum = strtod(yytext, NULL); \
            return FNUM_LITERAL; \
        } while(0)

typedef struct _file_name_stack {
    //int line_no;
    //int col_no;
    YY_BUFFER_STATE state;
    char *name;
    struct _file_name_stack *next;
} _file_name_stack;

int check_type(void);
void close_file(void);
void append_char(char ch);
void append_str(char *str);
void update_loc(void);

scanner_state_t scanner_state;

char buffer[1024*64];
int bidx = 0;
_file_name_stack *name_stack;
int num_errors = 0; // global updated by parser

%}
%x SQUOTES
%x DQUOTES
%x COMMENT
%option noinput nounput
%option yylineno
%option noyywrap

%%
    /* whitespace */
\n              { name_stack->state->yy_bs_lineno++; name_stack->state->yy_bs_column=0; }
[ \v\f\t\r]    {}

    /* recognize and ignore a C comments */
"/*"            { BEGIN(COMMENT); }
<COMMENT>"*/"   { BEGIN(INITIAL); }
<COMMENT>\n     { name_stack->state->yy_bs_lineno++; yylineno++; name_stack->state->yy_bs_column=0; }
<COMMENT>.      {}  /* eat everything in between */
"//".*          {} /* eat up until the newline */

    /* Keyword tokens */
"import"        { SET_TOKEN_STATE(IMPORT); }
"extern"        { SET_TOKEN_STATE(EXTERN); }
"const"         { SET_TOKEN_STATE(CONST); }
"static"        { SET_TOKEN_STATE(STATIC); }
"typedef"       { SET_TOKEN_STATE(TYPEDEF); }

"break"         { SET_TOKEN_STATE(BREAK); }
"continue"      { SET_TOKEN_STATE(CONTINUE); }
"return"        { SET_TOKEN_STATE(RETURN); }
"yield"         { SET_TOKEN_STATE(YIELD); }

"switch"        { SET_TOKEN_STATE(SWITCH); }
"case"          { SET_TOKEN_STATE(CASE); }
"default"       { SET_TOKEN_STATE(DEFAULT); }
"do"            { SET_TOKEN_STATE(DO); }
"while"         { SET_TOKEN_STATE(WHILE); }
"for"           { SET_TOKEN_STATE(FOR); }
"if"            { SET_TOKEN_STATE(IF); }
"else"          { SET_TOKEN_STATE(ELSE); }
"main"          { SET_TOKEN_STATE(MAIN); }

"float"         { SET_TOKEN_STATE(FLOAT); }
"int"           { SET_TOKEN_STATE(INT); }
"uint"          { SET_TOKEN_STATE(UINT); }
"bool"          { SET_TOKEN_STATE(BOOL); }
"void"          { SET_TOKEN_STATE(VOID); }
"string"        { SET_TOKEN_STATE(STRING); }
"tuple"         { SET_TOKEN_STATE(TUPLE); }
"struct"        { SET_TOKEN_STATE(STRUCT); }

"true"          { SET_TOKEN_STATE(TRUE); }
"false"         { SET_TOKEN_STATE(FALSE); }
"sizeof"        { SET_TOKEN_STATE(SIZEOF); }
"typeof"        { SET_TOKEN_STATE(TYPEOF); }

"..."           { SET_TOKEN_STATE(ELLIPSIS); }

    /* Operator tokens */
"&&"|"and"      { SET_TOKEN_STATE(AND_OP); }
"||"|"or"       { SET_TOKEN_STATE(OR_OP); }
"<="|"le"       { SET_TOKEN_STATE(LE_OP); }
">="|"ge"       { SET_TOKEN_STATE(GE_OP); }
"=="|"eq"       { SET_TOKEN_STATE(EQ_OP); }
"!="|"ne"       { SET_TOKEN_STATE(NE_OP); }
">>"            { SET_TOKEN_STATE(RIGHT_OP); }
"<<"            { SET_TOKEN_STATE(LEFT_OP); }

"&"             { SET_TOKEN_STATE('&'); }
"!"             { SET_TOKEN_STATE('!'); }
"~"             { SET_TOKEN_STATE('~'); }
"-"             { SET_TOKEN_STATE('-'); }
"+"             { SET_TOKEN_STATE('+'); }
"*"             { SET_TOKEN_STATE('*'); }
"/"             { SET_TOKEN_STATE('/'); }
"%"             { SET_TOKEN_STATE('%'); }
"<"             { SET_TOKEN_STATE('<'); }
">"             { SET_TOKEN_STATE('>'); }
"^"             { SET_TOKEN_STATE('^'); }
"|"             { SET_TOKEN_STATE('|'); }
"?"             { SET_TOKEN_STATE('?'); }

    /* Structural tokens */
";"             { SET_TOKEN_STATE(';'); }
"{"             { SET_TOKEN_STATE('{'); }
"}"             { SET_TOKEN_STATE('}'); }
","             { SET_TOKEN_STATE(','); }
":"             { SET_TOKEN_STATE(':'); }
"="             { SET_TOKEN_STATE('='); }
"("             { SET_TOKEN_STATE('('); }
")"             { SET_TOKEN_STATE(')'); }
"["             { SET_TOKEN_STATE('['); }
"]"             { SET_TOKEN_STATE(']'); }
"."             { SET_TOKEN_STATE('.'); }


[_A-Za-z][_0-9A-Za-z\.]*  { SET_IDENT_STATE(); }

    /* recognize an integer */
[0-9]+    { SET_INUM_STATE(); }

    /* recognize an unsigned number */
0[xX][0-9A-Fa-f]+ { SET_UNUM_STATE(); }

    /* recognize a float */
[-+]?[0-9]+\.[0-9]+([eE]*[-+]?[0-9]+)? { SET_FNUM_STATE(); }

    /* double quoted strings have escapes managed */
\"  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(DQUOTES);
    }

<DQUOTES>\" { SET_STRG_STATE(); }

    /* problem is that the short rule matches before the long one does */
<DQUOTES>\\n    { append_char('\n'); }
<DQUOTES>\\r    { append_char('\r'); }
<DQUOTES>\\t    { append_char('\t'); }
<DQUOTES>\\b    { append_char('\b'); }
<DQUOTES>\\f    { append_char('\f'); }
<DQUOTES>\\v    { append_char('\v'); }
<DQUOTES>\\\\   { append_char('\\'); }
<DQUOTES>\\\"   { append_char('\"'); }
<DQUOTES>\\\'   { append_char('\''); }
<DQUOTES>\\\?   { append_char('\?'); }
<DQUOTES>\\.    { append_char(yytext[1]); }
<DQUOTES>\\[0-7]{1,3} { append_char((char)strtol(yytext+1, 0, 8));  }
<DQUOTES>\\[xX][0-9a-fA-F]{1,3} { append_char((char)strtol(yytext+2, 0, 16));  }
<DQUOTES>[^\\\"\n]*  { append_str(yytext); }


    /* single quoted strings are absolute literals */
\'  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(SQUOTES);
    }

<SQUOTES>\' { SET_STRG_STATE(); }

<SQUOTES>[^\\'\n]*  { append_str(yytext); }
<SQUOTES>\\.    { append_str(yytext); }

    /* ignore characters such as '#' */
.   { }

<<EOF>> {

    if(name_stack != NULL) {
        DEBUG("closing file \"%s\"", name_stack->name);

        _file_name_stack *name = name_stack->next;
        yy_delete_buffer(name_stack->state);
        FREE(name_stack->name);
        FREE(name_stack);
        name_stack = name;

        if(name_stack != NULL)
            yy_switch_to_buffer(name_stack->state);
        else {
            DEBUG("calling yyterminate()");
            yyterminate();
        }
        memset(buffer, 0, sizeof(buffer));
    }
    else {
        DEBUG("<<EOF>> with no file open");
    }
    DEBUG("file closed");
    return END_OF_FILE;
}


%%

void open_file(const char *fname) {

    _file_name_stack *name;
    char* infile = (char*)find_import_file(fname);

    DEBUG("opening file: \"%s\"", infile);
    if(NULL == (name = CALLOC(1, sizeof(_file_name_stack))))
        scanner_error("cannot allocate memory for file stack");

    yyin = fopen(infile, "r");
    if(NULL == yyin) {
        scanner_error("cannot open the input file: \"%s\": %s", fname, strerror(errno));
        exit(1);
    }

    name->next = name_stack;
    name->name = infile;
    name->state = yy_create_buffer(yyin, YY_BUF_SIZE);
    name_stack = name;
    yy_switch_to_buffer(name_stack->state);
}

// these funcs support the string scanner
void append_char(char ch) {

    if((sizeof(buffer)-1) > (size_t)bidx) {
        buffer[bidx] = ch;
        bidx++;
    }
    else {
        scanner_error("buffer overrun");
    }
}

void append_str(char *str) {

    if((sizeof(buffer)-1) > (strlen(buffer) + strlen(str))) {
        strcat(buffer, str);
        bidx = strlen(buffer);
    }
    else {
        scanner_error("buffer overrun");
    }
}

void update_loc(void) {

    if(name_stack == NULL)
        return;

    for(char *s = yytext; *s != '\0'; s++) {
        name_stack->state->yy_bs_column++;
    }

}

// Tracking and global interface
char *get_file_name(void) {
    if(NULL != name_stack)
        return name_stack->name;
    else
        return "no open file";
}

int get_line_number(void) {
    if(NULL != name_stack)
        return name_stack->state->yy_bs_lineno;
    else
        return -1;
}

int get_col_number(void) {
    if(NULL != name_stack)
        return name_stack->state->yy_bs_column;
    else
        return -1;
}

/*
 * Return the token and copy the string that formed it if the parameter
 * is not NULL.
 */
int get_token(scanner_state_t* ss) {

    DEBUG("get_token(): file stack pointer: %p", name_stack);
    if(name_stack == NULL) {
        if(ss != NULL) {
            //memcpy((void*)ss, (void*)&scanner_state, sizeof(scanner_state_t));
            memset(ss, 0, sizeof(scanner_state_t));
            ss->token = END_OF_INPUT;
        }
        DEBUG("end token = %s", tok_to_strg(END_OF_INPUT));
        return END_OF_INPUT;
    }

    memset(buffer, 0, sizeof(buffer));
    memset(ss, 0, sizeof(scanner_state_t));
    int tok = yylex();
    if(tok == 0)
        tok = END_OF_INPUT;
    debug(10, "get_token() token = %s", tok_to_strg(tok));

    DEBUG("scanner ptr = %p", ss);
    if(ss != NULL) {
        memcpy((void*)ss, (void*)&scanner_state, sizeof(scanner_state_t));
    }

    return tok;
}

/*
 * Check the symbol table to discover if this is a defined type.
 */
int check_type(void) {
/*
* pseudo code --- this is what it should check
*
*   if (yytext == type_name)
*       return(NAMED_TYPE);
*
*   return(IDENTIFIER);
*/

/*
*   it actually will only return IDENTIFIER
*/

    return(IDENTIFIER);
}

