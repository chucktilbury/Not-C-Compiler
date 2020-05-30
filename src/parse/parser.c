
#include "common.h"
#include "internal.h"

void parse(void) {

    int tok;
    int finished = 0;
    parser_state_t ps;

    while(!finished) {
        tok = get_token(ps.ss);
        if(tok == IMPORT) {
            do_import(&ps);
        }
        else if(tok == TYPEDEF) {
            do_typedef(&ps);
        }
        else if(is_type(&ps)) {
            do_data_or_func_def(&ps);
        }
        else if(tok == END_OF_INPUT) {
            finished++;
        }
        else {
            syntax("expected import, data definition, or function definition, but got %s", tok_to_strg(tok));
            finished++;
        }
    }
}


