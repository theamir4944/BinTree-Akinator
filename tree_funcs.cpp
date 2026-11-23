#include <assert.h>
#include <stdio.h>


typedef char* nodval_t;
#define TYPE_SPECIFICATOR "%s"


const bool LEFT_TREE = false;
const bool RIGHT_TREE = true;
const size_t TREE_COMAND_LEN = 50;
size_t TREE_DUMP_COUNTER = 0;


struct node_t {
    
    node_t* parent_node;
    node_t* left_child;
    node_t* right_child;
    nodval_t value_node;
    
};


struct tree_t {
    
    node_t* tree_root;
    
};


node_t* bt_create_node(nodval_t value, tree_t* tree_info) {

    assert(tree_info != NULL);
    
    node_t* new_node_p = (node_t*) calloc (1, sizeof(node_t));
    
    if (new_node_p == NULL) {
        return NULL;
    }
    
    if (tree_info->tree_root == NULL) {
        tree_info->tree_root = new_node_p;
    }
    
    new_node_p->value_node = value;
    new_node_p->parent_node = NULL;
    new_node_p->left_child = NULL;
    new_node_p->right_child = NULL;
    
    return new_node_p;

}


void bt_destroy_node(node_t* node, tree_t* tree_info) {

    assert(node != NULL);
    assert(tree_info != NULL);

    if (node->left_child != NULL && node->right_child != NULL) {
        printf("The node isnt able to destroy\n");
        return;
    }

    bool is_left = ((node->parent_node)->left_child == node);
    node_t* the_child = (node->left_child) ? node->left_child : node->right_child;

    if (is_left) {
        (node->parent_node)->left_child = the_child;
    }
    else {
        (node->parent_node)->right_child = the_child;
    }

    if (the_child != NULL) {
        the_child->parent_node = node->parent_node;
    }

    node->value_node = 0;
    node->left_child = NULL;
    node->right_child = NULL;
    node->parent_node = NULL;

    free(node);

    return;

}


void bt_link_node(bool direction, node_t* new_child, node_t* parent) {

    assert(new_child != NULL);
    assert(parent != NULL);
    assert(new_child != parent);
    
    node_t* child_place = (direction) ? parent->right_child : parent->left_child;

    if (child_place != NULL) {
        printf("This place isnt free\n");
        return;
    }

    if (new_child->parent_node != NULL) {
        printf("This node already have a parent\n");
        return;
    }

    if (direction) {
        parent->right_child = new_child;
    }
    else {
        parent->left_child = new_child;
    }

    new_child->parent_node = parent;

    return;

}


void bt_form_header(FILE* dotfile, tree_t* tree_info) {

    fprintf(dotfile, "node [shape = Mrecord, fontcolor=\"black\", style=\"filled\", color=\"black\"]\n");
    fprintf(dotfile, "splines = line;\n");
    fprintf(dotfile, "nodesep = 1.0;\n");
    fprintf(dotfile, "ranksep = 1.0;\n");

    return;

}


#define fprintfindump(color)    fprintf(dotfile,\
                                "\"node%p\" [label = \"{parent: %p | value: " TYPE_SPECIFICATOR " | { <l> child_1: %p | <r> child_2: %p }}\", fillcolor=\"" color "\"]\n",\
                                node, node->parent_node, node->value_node, node->left_child, node->right_child);

void recursion_dump(FILE* dotfile, node_t* node, node_t* target_pointer = NULL) {

    if (node == NULL) { return; }

    if (node == target_pointer) {
            fprintfindump("yellow");
    }
    else {
        if (node->parent_node != NULL && node->parent_node->left_child == node) {
            fprintfindump("red");
        }
        else if (node->parent_node != NULL && node->parent_node->right_child == node) {
            fprintfindump("green");
        }
        else {
            fprintfindump("white");
        }
    }
    
    if (node->left_child) {
        fprintf(dotfile, "\"node%p\":l -> \"node%p\" [label = \"No\", color = \"red\"];\n", node, node->left_child);
        recursion_dump(dotfile, node->left_child, target_pointer);
    }
    if (node->right_child) {
        fprintf(dotfile, "\"node%p\":r -> \"node%p\" [label = \"Yes\", color = \"green\"];\n", node, node->right_child);
        recursion_dump(dotfile, node->right_child, target_pointer);
    }

    return;

}

#undef fprintfindump


void bt_dot_dump(tree_t* tree_info, node_t* target_pointer = NULL) {
    
    FILE* dotfile = fopen("tree.dot", "w");
    assert(dotfile != NULL);
    assert(tree_info != NULL);
    
    fprintf(dotfile, "digraph {\n\n");
    
    bt_form_header(dotfile, tree_info);
    
    recursion_dump(dotfile, tree_info->tree_root, target_pointer);
    
    fprintf(dotfile, "\n}");
    
    fclose(dotfile);

    return;
    
}


void bt_graphviz_dump(tree_t* tree_info, node_t* target_pointer = NULL) {

    bt_dot_dump(tree_info, target_pointer);
    char comand_string[TREE_COMAND_LEN] = { };
    sprintf(comand_string, "dot -Tpng tree.dot -o .\\dump_pictures\\graph%zd.png", TREE_DUMP_COUNTER);
    system(comand_string);

    return;

}


void paste_pngs(char* image_1, FILE* graph_file) {

    sprintf(image_1, ".\\dump_pictures\\graph%zd.png", TREE_DUMP_COUNTER);
    fprintf(graph_file, "<img src=\"%s\" alt=\"image\" />", image_1);

}


#define htmprintf(...) fprintf(graph_html, ##__VA_ARGS__)

void bt_make_html(tree_t* tree_info, node_t* target_pointer = NULL) {

    bt_graphviz_dump(tree_info, target_pointer);
    
    FILE* graph_html = (TREE_DUMP_COUNTER) ? fopen("graph.htm", "a") : fopen("graph.htm", "w");
    assert(graph_html != NULL);
    
    htmprintf("<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n");
    htmprintf("<style>\n");
    
    htmprintf("</style>\n</head>\n<body>\n");

    char image_1[TREE_COMAND_LEN] = { };

    paste_pngs(image_1, graph_html);
    htmprintf("<hr style=\"border: 2px dashed #000000; margin: 100px 0;\">\n");
    TREE_DUMP_COUNTER++;
    
    htmprintf("</body>\n</html>\n");
    fclose(graph_html);

    return;
    
}

#undef htmprintf


node_t* bt_goto_child(bool direction, node_t* parent) {

    assert(parent);
    if (direction) {
        return parent->right_child;
    }
    return parent->left_child;

}


node_t* bt_goto_parent(node_t* child) {

    assert(child);
    return child->parent_node;

}
