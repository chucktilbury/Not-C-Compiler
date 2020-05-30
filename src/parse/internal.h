#ifndef __INTERNAL_H__
#define __INTERNAL_H__

typedef struct _parser_state {

    scanner_state_t* ss;
} parser_state_t;

// types.c
int is_type(parser_state_t* ps);
int is_defined_type(parser_state_t* ps);

void do_import(parser_state_t* ps);

void do_typedef(parser_state_t* ps);

#endif
