
#include "common.h"
#include "time.h"

struct node_type_names {
    int type;
    const char* name;
};

struct node_type_names ntype_names[] = {
    {NO_NODE_TYPE, "NO_NODE_TYPE"},
    {ROOT_NODE, "ROOT_NODE"},
    {IMPORT_NODE, "IMPORT_NODE"},
    {TYPEDEF_NODE, "TYPEDEF_NODE"},
    {DATA_DEF_NODE, "DATA_DEF_NODE"},
    {FUNC_DEF_NODE, "FUNC_DEF_NODE"},
    {FUNC_DEF_PARM_NODE, "FUNC_DEF_PARM_NODE"},
    {EXPRESSION_ASSIGN_NODE, "EXPRESSION_ASSIGN_NODE"},
    {FUNC_PARAM_NODE, "FUNC_PARAM_NODE"},
    {FUNC_BODY_NODE, "FUNC_BODY_NODE"},
    {0, NULL}
};

const char* node_type_to_str(int type) {

    for(int i = 0; ntype_names[i].name != NULL; i++)
        if(ntype_names[i].type == type)
            return ntype_names[i].name;

    return "UNKNOWN_NODE";
}

/*
 * Dump the AST as a .DOT file.
 */
static void get_node_attributes(FILE* fp, ast_node_t* node) {

    const ast_attr_map_t* map;
    const char* key;

    for(key = iterate_hash_table(node->attribs, 1); key != NULL; key = iterate_hash_table(node->attribs, 0)) {
        map = attr_name_map(key);
        switch(map->stype) {
            case NUM_ATTR: {
                    int var;
                    get_node_attrib(node, map->type, &var, sizeof(int));
                    switch(map->type) {
                        case DATA_TYPE_ATTR:
                            fprintf(fp, " %s: %s\\n", key, tok_to_strg(var));
                            break;
                        default:
                            fprintf(fp, " %s: %d\\n", key, var);
                    }
                }
                break;
            case STR_ATTR: {
                    size_t size = get_node_attr_size(node, map->type);
                    char* str = MALLOC(size);
                    get_node_attrib(node, map->type, (void*)str, size);
                    fprintf(fp, " %s: %s\\n", key, str);
                    FREE((void*)str);
                }
                break;
            case STRUCT_ATTR: {
                    fprintf(fp, " %s: <struct>\\n", key);
                }
                break;
            default:
                fprintf(fp, " %s: <unknown stype>\\n", key);
        }
    }
}

static void dump_walk_ast(FILE* fp, ast_node_t* node, const char* prev_name) {

    char name[30];
    ast_node_t* n;

    sprintf(name, "node_%p", node);
    fprintf(fp, "    %s [label=\"{type: %s\\nattributes: \\n", name, node_type_to_str(node->node_type));
    get_node_attributes(fp, node);
    fprintf(fp, "}\"];\n");
    if(prev_name != NULL)
        fprintf(fp, "    %s -> %s;\n", prev_name, name);

    fflush(fp);

    while(NULL != (n = next_member(node)))
        dump_walk_ast(fp, n, name);

}

/*
 * Dump the specified AST as a ".dot" file to the file name specified.
 * https://www.graphviz.org/doc/info/lang.html
 */
void dump_ast(ast_node_t* root, const char* file) {

    time_t t = time(NULL);
    FILE* outfile = fopen(file, "w");

    if(outfile == NULL) {
        fprintf(stderr, "Graph Error: Cannot open %s for writing: %s\n", file, strerror(errno));
        return;
    }

    fprintf(outfile, "// file name \"%s\" dumped %s", get_file_name(), ctime(&t));
    fprintf(outfile, "digraph Source_AST_Tree {\n");
    fprintf(outfile, "    label=\"file name %s dumped %s\"", get_file_name(), ctime(&t));
    fprintf(outfile, "    node [style=\"rounded,filled\" shape=record]\n\n");

    dump_walk_ast(outfile, root, NULL); // root only has children

    fprintf(outfile, "\n}\n");

    fclose(outfile);
}

