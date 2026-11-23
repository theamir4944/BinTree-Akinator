#include "treelib.cpp"
#include "outer_wrapper.cpp"


void akinator_guessing(node_t*, tree_t*);
void get_comparison(tree_t*, const char*, const char*);
void get_full_defenition(tree_t* tree_info, const char* item);


int main() {
    
    InitWindow(screen_width, screen_height, "raylib window");
    SetTargetFPS(60);
    
    int user_ans = raylib_first_buttons_choice("Choose using-mode");
    
    char* datafile = 0;
    tree_t* tree_info = NULL;
    
    if (user_ans > 0 && user_ans <= 3) {
        
        datafile = raylib_enter_text("Enter name of data-file");
        tree_info = read_text_tree(datafile);
        
        if (! tree_info) {
            raylib_show_text("Cant open the file");
            return 1;
        }
        
    }

    switch (user_ans) {
        case 1: {
            akinator_guessing(tree_info->tree_root, tree_info);
            bt_dot_dump(tree_info);
            write_text_tree(datafile, tree_info);
            break;
        }
        case 2: {
            char* item = raylib_enter_text("Choose the item to get definition");
            
            get_full_defenition(tree_info, item);
            break;
        }
        case 3: {
            char* item_1 = raylib_enter_text("Choose the 1st item to get definition", 35);
            char* item_2 = raylib_enter_text("Choose the 2nd item to get definition", 35);
            get_comparison(tree_info, item_1, item_2);
            break;
        }
        default: {
            raylib_show_text("Error!\nIncorrect mode");
            printf("User-ans value: %d\n", user_ans);
            return 2;
        }

    }

    CloseWindow();

    bt_make_html(tree_info);
    
    return 0;
    
}


bool get_user_answer() {
    
    printf("[Y/n]: ");
    int newchar = getchar();
    while (getchar() != '\n') {
        ;
    }
    return (newchar == 'N' || newchar == 'n') ? false : true;
    
}


void get_new_node(node_t* node, tree_t* main_tree) {
    
    char* new_item = raylib_enter_text("So, what was it?\n");
    
    char maintext[MAX_TEXT_SIZE] = { };
    sprintf(maintext, "How is %s\n different from\n%s?\n", new_item, node->value_node);
    char* difference = raylib_enter_text(maintext);
    
    node_t* left_child = bt_create_node(node->value_node, main_tree);
    node_t* right_child = bt_create_node(new_item, main_tree);
    node->value_node = difference;
    
    bt_link_node(LEFT_TREE, left_child, node);
    bt_link_node(RIGHT_TREE, right_child, node);
    
    return;
    
}


void akinator_guessing(node_t* during_node, tree_t* main_tree) {
    
    if (during_node == NULL) {
        raylib_show_text("Error: broken tree (NULL node)");
        return;
    }
    
    if (during_node->left_child == NULL && during_node->right_child == NULL) {
        
        char maintext[MAX_TEXT_SIZE] = { };
        sprintf(maintext, "%s?\n", during_node->value_node);
        
        if (raylib_button_bool_ans(maintext)) {
            raylib_show_text("I have won again!");
        }
        else {
            get_new_node(during_node, main_tree);
        }
        
    } 
    else {
        
        char maintext[MAX_TEXT_SIZE] = { };
        sprintf(maintext, "%s?\n", during_node->value_node);
        
        bool direction = raylib_button_bool_ans(maintext);
        node_t* next_node = bt_goto_child(direction, during_node);
        akinator_guessing(next_node, main_tree);
        
    }
    
}


void get_full_defenition(tree_t* tree_info, const char* item) {
    
    stack_t main_stack = { };
    stack_constructor(&main_stack, 1);
    get_def_tostack(tree_info, item, &main_stack);
    
    const char* val = NULL;
    
    char maintext[MAX_TEXT_SIZE] = { };
    char* maintext_copy = maintext;
    maintext_copy += sprintf(maintext_copy, "\n\n%s defenition:\n", item);
    
    while (main_stack.size != 0) {
        stack_pop(&main_stack, &val);
        maintext_copy += sprintf(maintext_copy, "%s", val);
    }
    
    raylib_show_text(maintext);
    
    return;
    
}


void get_comparison(tree_t* tree_info, const char* item_1, const char* item_2) {
    
    stack_t stack_1 = { };
    stack_t stack_2 = { };
    
    stack_constructor(&stack_1, 1);
    stack_constructor(&stack_2, 1);
    
    get_def_tostack(tree_info, item_1, &stack_1);
    get_def_tostack(tree_info, item_2, &stack_2);
    
    const char* param_1 = NULL;
    const char* param_2 = NULL;
    
    stack_pop(&stack_1, &param_1);
    stack_pop(&stack_2, &param_2);
    
    char maintext[MAX_TEXT_SIZE] = { };
    char* maintext_copy = maintext;
    
    if (param_1 == NULL || param_2 == NULL) {
        raylib_show_text("Error! No such item");
        return;
    }
    
    maintext_copy += sprintf(maintext_copy, "Same:");
    
    while (param_1 == param_2 && stack_1.size && stack_2.size) {
        maintext_copy += sprintf(maintext_copy, "%s", param_1);
        stack_pop(&stack_1, &param_1);
        stack_pop(&stack_2, &param_2);
    }
    
    maintext_copy += sprintf(maintext_copy, "\nDifferent:");
    
    maintext_copy += sprintf(maintext_copy, "\nFor %s:\n", item_1);
    while(stack_1.size) {
        maintext_copy += sprintf(maintext_copy, "%s", param_1);
        stack_pop(&stack_1, &param_1);
    }
    maintext_copy += sprintf(maintext_copy, "%s", param_1);
    
    maintext_copy += sprintf(maintext_copy, "\n\nFor %s:\n", item_2);
    while(stack_2.size) {
        maintext_copy += sprintf(maintext_copy, "%s", param_2);
        stack_pop(&stack_2, &param_2);
    }
    maintext_copy += sprintf(maintext_copy, "%s", param_2);
    
    raylib_show_text(maintext, 30);
    
    return;
    
}
