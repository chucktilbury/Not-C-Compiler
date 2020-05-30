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

// string buffer to get token from
//static char strbuf[1024*64];

typedef struct _file_name_stack {
    int line_no;
    int col_no;
    char *name;
    struct _file_name_stack *next;
} _file_name_stack;

void count(void);
static char buffer[1024*64];
static int bidx = 0;
static _file_name_stack *name_stack;
int num_errors = 0; // global updated by parser

// these funcs support the string scanner
static void append_char(char ch) {
    if((sizeof(buffer)-1) > (size_t)bidx) {
        buffer[bidx] = ch;
        bidx++;
    }
    else {
        scanner_error("buffer overrun");
    }
}

static void append_str(char *str) {
    if((sizeof(buffer)-1) > (strlen(buffer) + strlen(str))) {
        strcat(buffer, str);
        bidx = strlen(buffer);
    }
    else {
        scanner_error("buffer overrun");
    }
}

static void update_loc(void){

    if(name_stack == NULL)
        return;

    for(char *s = yytext; *s != '\0'; s++) {
        name_stack->col_no++;
    }

}

/*  */
int check_type(void);

#define YY_USER_ACTION update_loc();
/*
#define TOKEN(t)    do{yylval.token = (t); return(t); }while(0)
#define IDENT()     do{yylval.str = strdup(yytext); return(check_type()); }while(0)
#define DEF_UNUM()  do{yylval.value.unum = strtol(yytext, NULL, 16); return(UNUM_LITERAL); }while(0)
#define DEF_INUM()  do{yylval.value.inum = strtol(yytext, NULL, 10); return(INUM_LITERAL); }while(0)
#define DEF_FNUM()  do{yylval.value.fnum = strtod(yytext, NULL); return(FNUM_LITERAL); }while(0)
#define STRG()      do{yylval.str = strdup(buffer); BEGIN(INITIAL); return(STRING_LITERAL); }while(0)
*/

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

static scanner_state_t scanner_state;


%}
%x SQUOTES
%x DQUOTES
%x COMMENT
%option noinput nounput
%option yylineno
%option noyywrap

%%
    /* whitespace */
\n              { name_stack->line_no++; name_stack->col_no=0; }
[ \v\f\t\r]    {}

    /* recognize and ignore a C comments */
"/*"            { BEGIN(COMMENT); }
<COMMENT>"*/"   { BEGIN(INITIAL); }
<COMMENT>\n     { name_stack->line_no++; yylineno++; name_stack->col_no=0; }
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

        DEBUG("closing file \"%s\"", name_stack->name);
        _file_name_stack *name = name_stack->next;
        free(name_stack->name);
        free(name_stack);
        name_stack = name;

        memset(buffer, 0, sizeof(buffer));
        yypop_buffer_state();
        if(!YY_CURRENT_BUFFER) {
            yyterminate();
        }
    }


%%

void open_file(char *fname) {

    _file_name_stack *name;
// TODO: add a check to make sure that any file is opened exactly once.
// TODO: add logic to allow for files to be searched for using a command parameter.

    DEBUG("opening file: \"%s\"", fname);
    if(NULL == (name = calloc(1, sizeof(_file_name_stack))))
        scanner_error("cannot allocate memory for file stack");

    if(NULL == (name->name = strdup(fname)))
        scanner_error("cannot allocate memory for file stack name");

    name->next = name_stack;
    name->line_no = 1;
    name->col_no = 1;
    name_stack = name;

    yyin = fopen(fname, "r");
    if(NULL == yyin) {
        scanner_error("cannot open the input file: \"%s\": %s", fname, strerror(errno));
        exit(1);
    }

    yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
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
        return name_stack->line_no;
    else
        return -1;
}

int get_col_number(void) {
    if(NULL != name_stack)
        return name_stack->col_no;
    else
        return -1;
}

/*const char *get_tok_str(void) {
    return strbuf;
}*/

/*
 * Return the token and copy the string that formed it if the parameter
 * is not NULL.
 */
int get_token(scanner_state_t* ss) {

    memset(buffer, 0, sizeof(buffer));
    memset(ss, 0, sizeof(scanner_state_t));
    int tok = yylex();

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


#if 0
yywrap()
{
    return(1);
}


comment()
{
    char c, c1;

loop:
    while ((c = input()) != '*' && c != 0)
        putchar(c);

    if ((c1 = input()) != '/' && c != 0)
    {
        unput(c1);
        goto loop;
    }

    if (c != 0)
        putchar(c1);
}


int column = 0;

void count()
{
    int i;

    for (i = 0; yytext[i] != '\0'; i++)
        if (yytext[i] == '\n')
            column = 0;
        else if (yytext[i] == '\t')
            column += 8 - (column % 8);
        else
            column++;

    ECHO;
}
#endif
