#include <ctype.h>
#include <string.h>

#include "tree_funcs.cpp"
#include "stack.cpp"


char* read_text_value(FILE* stream) {
    
    assert(stream != NULL);
    
    size_t index = 0;
    size_t buf_cap = 1;
    
    char* res_buf = (char*) calloc (buf_cap, sizeof(char));
    char* buf_copy = res_buf;
    if (res_buf == NULL) {
        return NULL;
    }
    
    int new_char = fgetc(stream);
    
    while (isspace(new_char) && new_char != EOF) {
        new_char = fgetc(stream);
    }
    
    if (new_char == '(' || new_char == ')') {
        ungetc(new_char, stream);
        free(res_buf);
        return NULL;
    }
    
    if (new_char == EOF) {
        free(res_buf);
        return NULL;
    }
    
    while (new_char != EOF && new_char != '(' && new_char != ')') {
        
        if (index >= buf_cap - 1) {
            
            buf_copy = (char*) realloc (res_buf, 2 * buf_cap);
            
            if (buf_copy != NULL) {
                res_buf = buf_copy;
                buf_cap *= 2;
            }
            else {
                printf("Cant raise buffer\n");
                res_buf[index] = '\0';
                return res_buf;
            }
            
        }
        
        res_buf[index++] = new_char;
        new_char = fgetc(stream);
        
    }
    
    if (new_char == '(' || new_char == ')') {
        ungetc(new_char, stream);
    }
    
    res_buf[index] = '\0';
    return res_buf;
    
}


void preord_text_reading(FILE* stream, node_t* node, tree_t* tree) {
    
    assert(stream != NULL);
    assert(tree != NULL);
    
    if (node == NULL) {
        return;
    }
    
    nodval_t new_value = read_text_value(stream);
    if (! new_value) {
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
        preord_text_reading(stream, left_child, tree);
    }

    read_syms = 0;
    fscanf(stream, "(%n", &read_syms);
    if (read_syms) {
        node_t* right_child = bt_create_node(0, tree);
        bt_link_node(RIGHT_TREE, right_child, node);
        preord_text_reading(stream, right_child, tree);
    }
    
    fscanf(stream, ")");
    return;
    
}


tree_t* read_text_tree(const char* filename) {
    
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
    preord_text_reading(stream, new_node, main_tree);
    
    return main_tree;
    
}


void preord_text_writing(FILE* stream, node_t* node) {
    
    assert(stream != NULL);
    
    fprintf(stream, "(");
    
    if (node != NULL) {
        fprintf(stream, TYPE_SPECIFICATOR, node->value_node);
        preord_text_writing(stream, node->left_child);
        preord_text_writing(stream, node->right_child);
    }
    
    fprintf(stream, ")");
    
    return;
    
}


void write_text_tree(const char* filename, tree_t* thetree) {
    
    assert(filename != NULL);
    assert(thetree != NULL);
    
    FILE* stream = fopen(filename, "w");
    if (stream == NULL) {
        printf("Cant open the file\n");
        return;
    }
    
    preord_text_writing(stream, thetree->tree_root);
    
    return;
    
}


node_t* seek_node(node_t* current_node, const char* value) {
    
    if (current_node == NULL) {
        return NULL;
    }
    
    if (strcmp(current_node->value_node, value) == 0) {
        return current_node;
    }
    
    node_t* left_res = seek_node(current_node->left_child, value);
    node_t* right_res = seek_node(current_node->right_child, value);
    
    return (left_res == NULL) ? right_res : left_res;
    
}


void get_def_tostack(tree_t* main_tree, const char* value, stack_t* defstack) {
    
    assert(value);
    assert(defstack);
    
    node_t* res_node_p = seek_node(main_tree->tree_root, value);
    
    if (res_node_p == NULL) {
        printf("This item isnt in akinators data\n");
        return;
    }
    
    while (res_node_p->parent_node != NULL) {

        stack_push(defstack, res_node_p->parent_node->value_node);
        if (res_node_p->parent_node->left_child == res_node_p) {
            stack_push(defstack, "Not ");
        }
        stack_push(defstack, "\n");
        res_node_p = res_node_p->parent_node;
        
    }

    return;
    
}
