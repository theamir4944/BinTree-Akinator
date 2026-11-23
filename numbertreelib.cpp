#include "tree_funcs.cpp"


enum order_t {

    PREORDER = 0,
    INORDER = 1,
    POSTORDER = 2

};


void preord_rec_reading(FILE* stream, node_t* node, tree_t* tree) {

    assert(stream != NULL);
    assert(tree != NULL);

    if (node == NULL) {
        return;
    }

    nodval_t new_value = 0;
    if (! fscanf(stream, TYPE_SPECIFICATOR, &new_value)) {
        bt_destroy_node(node, tree);
        int next_char = fgetc(stream);
        while (next_char != '(' && next_char != ')') {
            next_char = fgetc(stream);
        }
        return;
    }
    node->value_node = new_value;

    int read_syms = 0;
    fscanf(stream, "(%n", &read_syms);
    if (read_syms) {
        node_t* left_child = bt_create_node(0, tree);
        bt_link_node(LEFT_TREE, left_child, node);
        preord_rec_reading(stream, left_child, tree);
    }

    read_syms = 0;
    fscanf(stream, "(%n", &read_syms);
    if (read_syms) {
        node_t* right_child = bt_create_node(0, tree);
        bt_link_node(RIGHT_TREE, right_child, node);
        preord_rec_reading(stream, right_child, tree);
    }

    fscanf(stream, ")");
    return;

}


void inord_rec_reading(FILE* stream, node_t* node, tree_t* tree) {

    assert(stream != NULL);
    assert(tree != NULL);

    if (node == NULL) {
        return;
    }

    int read_syms = 0;
    fscanf(stream, "(%n", &read_syms);
    if (read_syms) {
        node_t* left_child = bt_create_node(0, tree);
        bt_link_node(LEFT_TREE, left_child, node);
        inord_rec_reading(stream, left_child, tree);
    }

    nodval_t new_value = 0;
    if (! fscanf(stream, TYPE_SPECIFICATOR, &new_value)) {
        bt_destroy_node(node, tree);
        int next_char = fgetc(stream);
        while (next_char != '(' && next_char != ')') {
            next_char = fgetc(stream);
        }
        return;
    }
    node->value_node = new_value;

    read_syms = 0;
    fscanf(stream, "(%n", &read_syms);
    if (read_syms) {
        node_t* right_child = bt_create_node(0, tree);
        bt_link_node(RIGHT_TREE, right_child, node);
        inord_rec_reading(stream, right_child, tree);
    }

    fscanf(stream, ")");
    return;

}


void postord_rec_reading(FILE* stream, node_t* node, tree_t* tree) {

    assert(stream != NULL);
    assert(tree != NULL);

    if (node == NULL) {
        return;
    }

    int read_syms = 0;
    fscanf(stream, "(%n", &read_syms);
    if (read_syms) {
        node_t* left_child = bt_create_node(0, tree);
        bt_link_node(LEFT_TREE, left_child, node);
        postord_rec_reading(stream, left_child, tree);
    }

    read_syms = 0;
    fscanf(stream, "(%n", &read_syms);
    if (read_syms) {
        node_t* right_child = bt_create_node(0, tree);
        bt_link_node(RIGHT_TREE, right_child, node);
        postord_rec_reading(stream, right_child, tree);
    }
    
    nodval_t new_value = 0;
    if (! fscanf(stream, TYPE_SPECIFICATOR, &new_value)) {
        bt_destroy_node(node, tree);
        int next_char = fgetc(stream);
        while (next_char != '(' && next_char != ')') {
            next_char = fgetc(stream);
        }
        return;
    }
    node->value_node = new_value;

    fscanf(stream, ")");
    return;

}


void (* order_read_p[3])(FILE*, node_t*, tree_t*) = {
    &preord_rec_reading, &inord_rec_reading, &postord_rec_reading
};


tree_t* read_tree(const char* filename, order_t order_type) {

    assert(filename != NULL);

    tree_t* main_tree = (tree_t*) calloc (1, sizeof(tree_t));
    if (main_tree == NULL) {
        printf("No memory\n");
        return NULL;
    }
    node_t* new_node = bt_create_node(0, main_tree);

    FILE* stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("Cant open the file\n");
        return NULL;
    }

    fscanf(stream, "(");
    (*order_read_p[order_type])(stream, main_tree->tree_root, main_tree);

    return main_tree;

}


void preord_rec_writing(FILE* stream, node_t* node) {

    assert(stream != NULL);

    fprintf(stream, "(");

    if (node != NULL) {
        fprintf(stream, TYPE_SPECIFICATOR, node->value_node);
        preord_rec_writing(stream, node->left_child);
        preord_rec_writing(stream, node->right_child);
    }

    fprintf(stream, ")");

    return;

}


void inord_rec_writing(FILE* stream, node_t* node) {

    assert(stream != NULL);

    fprintf(stream, "(");

    if (node != NULL) {
        inord_rec_writing(stream, node->left_child);
        fprintf(stream, TYPE_SPECIFICATOR, node->value_node);
        inord_rec_writing(stream, node->right_child);
    }

    fprintf(stream, ")");

    return;

}


void postord_rec_writing(FILE* stream, node_t* node) {

    assert(stream != NULL);

    fprintf(stream, "(");

    if (node != NULL) {
        postord_rec_writing(stream, node->left_child);
        postord_rec_writing(stream, node->right_child);
        fprintf(stream, TYPE_SPECIFICATOR, node->value_node);
    }

    fprintf(stream, ")");

    return;

}


void (* order_write_p[3])(FILE*, node_t*) = {
    &preord_rec_writing, &inord_rec_writing, &postord_rec_writing
};


void write_tree(const char* filename, order_t order_type, tree_t* thetree) {

    assert(filename != NULL);
    assert(thetree != NULL);

    FILE* stream = fopen(filename, "w");
    if (stream == NULL) {
        printf("Cant open the file\n");
        return;
    }

    (*order_write_p[order_type])(stream, thetree->tree_root);

    return;

}