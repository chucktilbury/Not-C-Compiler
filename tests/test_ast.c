
#include "common.h"

// build  as:
// gcc -Wall -Wextra -g test_ast.c -I../src/include -L../lib/ -last -lutils -lparser

int main(void) {

    char buffer[25];
    int number;
    ast_node_t* root, *new_root;

    root = create_node();
    strcpy(buffer, "root");
    add_attribute(root, "NAME", buffer, strlen(buffer)+1);
    number = 1;
    add_attribute(root, "LINE", &number, sizeof(number));
    number = 1;
    add_attribute(root, "COLUMN", &number, sizeof(number));

    new_root = create_node();
    add_node_as_child(root, new_root);
    strcpy(buffer, "first_child");
    add_attribute(new_root, "NAME", buffer, strlen(buffer)+1);
    number = 2;
    add_attribute(new_root, "LINE", &number, sizeof(number));
    number = 3;
    add_attribute(new_root, "COLUMN", &number, sizeof(number));

    new_root = create_node();
    add_node_as_child(root, new_root);
    strcpy(buffer, "second_child");
    add_attribute(new_root, "NAME", buffer, strlen(buffer)+1);
    number = 4;
    add_attribute(new_root, "LINE", &number, sizeof(number));
    number = 5;
    add_attribute(new_root, "COLUMN", &number, sizeof(number));

    new_root = create_node();
    add_node_as_sibling(root, new_root);
    strcpy(buffer, "first_sibling");
    add_attribute(new_root, "NAME", buffer, strlen(buffer)+1);
    number = 6;
    add_attribute(new_root, "LINE", &number, sizeof(number));
    number = 7;
    add_attribute(new_root, "COLUMN", &number, sizeof(number));

    new_root = create_node();
    add_node_as_sibling(root, new_root);
    strcpy(buffer, "second_sibling");
    add_attribute(new_root, "NAME", buffer, strlen(buffer)+1);
    number = 8;
    add_attribute(new_root, "LINE", &number, sizeof(number));
    number = 9;
    add_attribute(new_root, "COLUMN", &number, sizeof(number));



    dump_ast(root, "testfile.dot");

    return 0;
}
