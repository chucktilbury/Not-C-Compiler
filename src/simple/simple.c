
#include <argp.h>

#include "common.h"
#include "parser.h"

#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"
#include "llvm-c/Core.h"

BEGIN_CONFIG
    CONFIG_NUM("-v", "VERBOSE", "Set the verbosity from 0 to 50", 0, 0)
    CONFIG_STR("-o", "OUTFILE", "Specify the file name to output", 0, "output.bc")
    CONFIG_LIST("-p", "FPATH", "Specify directories to search for imports", 0, ".:include")
    CONFIG_STR("-d", "DUMP_FILE", "Specify the file name to dump the AST into", 0, "ast_dump.dot")
END_CONFIG


LLVMModuleRef Module;
LLVMContextRef Context;
LLVMTypeRef int32bit;
LLVMTypeRef int32bitptr;

memory_system_t* memory_system;

int main(int argc, char **argv)
{
    ast_node_t* root;

    init_memory_system();
    configure(argc, argv);

    int verbose = GET_CONFIG_NUM("VERBOSE");
    init_errors(verbose, stdout);

    for(char* str = iterate_config("INFILES"); str != NULL; str = iterate_config("INFILES"))
    {
        root = parse(str);
    }

    int errors = get_num_errors();
    if(errors != 0)
        printf("\nparse failed: %d errors: %d warnings\n", errors, get_num_warnings());
    else
        printf("\nparse succeeded: %d errors: %d warnings\n", errors, get_num_warnings());

    const char* dump_file = GET_CONFIG_STR("DUMP_FILE");
    if(verbose > 5 && root && dump_file)
        dump_ast(root, dump_file);

    destroy_memory_system();

    return errors;
}
