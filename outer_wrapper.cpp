#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


const int MAX_INPUT_CHARS = 20;
const int MAX_TEXT_SIZE = 200;
const int screen_width = 800;
const int screen_height = 450;


#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)


bool IsAnyKeyPressed();
char* raylib_enter_text(const char* maintext, int size);
int raylib_button_bool_ans(const char* maintext);
void raylib_show_text(const char* maintext, int size);


bool IsAnyKeyPressed() {

    int key = GetKeyPressed();
    return ((key >= 32) && (key <= 126)) ? true : false;
    
}


int substrlen(char* string, int start_ind, int length, int textsize) {

    char old_char = string[start_ind + length];
    string[start_ind + length] = '\0';
    int result = MeasureText(string + start_ind, textsize);
    string[start_ind + length] = old_char;
    return result;

}


struct input_str_info {

    Rectangle text_square;
    char* thetext;
    int letters_counter;
    int counter_frame;
    int cursor_pos;
    int key;

};


void do_backspace(input_str_info* cur_str) {

    cur_str->letters_counter--;
    cur_str->cursor_pos--;
    cur_str->letters_counter = (cur_str->letters_counter >= 0) ? cur_str->letters_counter : 0;
    strcpy(cur_str->thetext + cur_str->cursor_pos, cur_str->thetext + cur_str->cursor_pos + 1);
    cur_str->thetext[cur_str->letters_counter] = '\0';

}


void letter_key_processing(input_str_info* cur_str) {

    cur_str->key = GetCharPressed();
    while (cur_str->key > 0) {
        if ((cur_str->key >= ' ') && (cur_str->key <= '}') && (cur_str->letters_counter < MAX_INPUT_CHARS)) {
            memmove(cur_str->thetext + cur_str->cursor_pos + 1, cur_str->thetext + cur_str->cursor_pos, cur_str->letters_counter - cur_str->cursor_pos + 1);
            cur_str->thetext[cur_str->cursor_pos++] = (char)cur_str->key;
            cur_str->thetext[++cur_str->letters_counter] = '\0';
        }
        cur_str->key = GetCharPressed();
    }

}


bool key_processing(input_str_info* cur_str) {

    if (IsKeyPressed(KEY_ENTER)) {
        return false;
    }

    letter_key_processing(cur_str); 
    int current_key = GetKeyPressed();

    if (IsKeyPressed(KEY_BACKSPACE)) { // Scan-codes
        do_backspace(cur_str);
    }

    if (IsKeyPressed(KEY_LEFT)) {
        cur_str->cursor_pos = MAX(0, cur_str->cursor_pos - 1);
    }

    if (IsKeyPressed(KEY_RIGHT)) {
        cur_str->cursor_pos = MIN(cur_str->letters_counter, cur_str->cursor_pos + 1);
    }

    if (IsKeyPressed(KEY_LEFT_ALT) || current_key == 327) {
        cur_str->cursor_pos = 0;
    }

    if (IsKeyPressed(KEY_RIGHT_ALT) || current_key == 321) {
        cur_str->cursor_pos = cur_str->letters_counter;
    }

    return true;

}


void enter_text_drawing(input_str_info* cur_str, const char* maintext, int size = 35) {

    DrawText(maintext, 5, 5, size, GREEN);
    DrawRectangleRec(cur_str->text_square, BLACK);
    DrawRectangleLines((int) cur_str->text_square.x, (int) cur_str->text_square.y, (int) cur_str->text_square.width, (int) cur_str->text_square.height, GREEN);
    DrawText(cur_str->thetext, (int) cur_str->text_square.x + 5, (int) cur_str->text_square.y + 8, 40, GREEN);

    char input_chars_text[30] = { };
    sprintf(input_chars_text, "Input Chars: %d / %d", cur_str->letters_counter, MAX_INPUT_CHARS);
    DrawText(input_chars_text, (int) cur_str->text_square.x, (int) cur_str->text_square.y - 20, 15, DARKGRAY);

    if (((cur_str->counter_frame / 20) % 2) == 0) {
        if (cur_str->letters_counter < MAX_INPUT_CHARS) {
            DrawText("|", (int) cur_str->text_square.x + 8 + substrlen(cur_str->thetext, 0, cur_str->cursor_pos, 40), (int) cur_str->text_square.y + 12, 40, GREEN);
        }
        else {
            DrawText("Press BACKSPACE to delete chars...", screen_width / 2 - 130, screen_height - 30, 25, RED);
        }
    }

}


char* raylib_enter_text(const char* maintext, int size = 35) {

    input_str_info cur_str = { };
    cur_str.thetext = (char*) calloc (MAX_INPUT_CHARS + 1, sizeof(char));
    cur_str.text_square = {screen_width / 2 - 200, 180, 450, 50};

    while (!WindowShouldClose()) {

        if(key_processing(&cur_str) == false) {
            break;
        }
        cur_str.counter_frame++;

        BeginDrawing();

        ClearBackground(BLACK);
        enter_text_drawing(&cur_str, maintext, size);

        EndDrawing();

    }

    PollInputEvents();
    return cur_str.thetext;

}


enum button_pud {

    BUTTON_NONE = -1,
    BUTTON_NO,
    BUTTON_YES,
    BUTTON_PLAY,
    BUTTON_DEFINE,
    BUTTON_COMPARE,
    BUTTON_MAX

};


int raylib_button_bool_ans(const char* maintext) {

    float button_width = 160;
    float button_height = 100;
    float button_space = 40;
    float button_x = 220;
    float button_y = 300;

    Rectangle buttons[2] = {
        {button_x, button_y, button_width, button_height},
        {button_x + button_width + button_space, button_y, button_width, button_height}
    };

    const char* buttons_text[2] = {"NO", "YES"};
    Color button_colors[2] = {RED, GREEN};
    int pressed_button = BUTTON_NONE;
    Vector2 cursor_position = {0, 0};

    while (!WindowShouldClose() && pressed_button == BUTTON_NONE) {

        cursor_position = GetMousePosition();
        pressed_button = BUTTON_NONE;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < 2; i++) {
                if (CheckCollisionPointRec(cursor_position, buttons[i])) {
                    pressed_button = i;
                    break;
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);
        DrawText(maintext, 240, 140, 35, GREEN);

        for (int i = 0; i < 2; i++) {

            DrawRectangleRec(buttons[i], (i == pressed_button)? GRAY : DARKGRAY);
            DrawRectangleLinesEx(buttons[i], 2, RAYWHITE);

            int text_delta_y = 40;
            int text_delta_x = MeasureText(buttons_text[i], text_delta_y);
            int text_x = (int) (buttons[i].x + (buttons[i].width - text_delta_x) / 2);
            int text_y = (int) (buttons[i].y + (buttons[i].height - text_delta_y) / 2);
            DrawText(buttons_text[i], text_x, text_y, text_delta_y, button_colors[i]);
            
        }

        EndDrawing();

    }

    return pressed_button;

}


int raylib_first_buttons_choice(const char* maintext) {

    float button_width = 500;
    float button_height = 60;
    float button_space = 10;
    float button_x = 150;
    float button_y = 220;

    Rectangle buttons[3] = {
        {button_x, button_y, button_width, button_height},
        {button_x, button_y + button_height + button_space, button_width, button_height},
        {button_x, button_y + 2 * button_height + 2 * button_space, button_width, button_height}
    };

    const char* buttons_text[3] = {"PLAY AKINATOR", "GET DEFINITION", "COMPARE ITEMS"};
    Color button_color = GREEN;
    int pressed_button = BUTTON_NONE;
    Vector2 cursor_position = {0, 0};

    while (!WindowShouldClose() && pressed_button == BUTTON_NONE) {

        cursor_position = GetMousePosition();
        pressed_button = BUTTON_NONE;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < 3; i++) {
                if (CheckCollisionPointRec(cursor_position, buttons[i])) {
                    pressed_button = i;
                    break;
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);
        DrawText(maintext, 240, 140, 35, GREEN);

        for (int i = 0; i < 3; i++) {

            DrawRectangleRec(buttons[i], (i == pressed_button)? GRAY : DARKGRAY);
            DrawRectangleLinesEx(buttons[i], 2, RAYWHITE);

            int text_delta_y = 40;
            int text_delta_x = MeasureText(buttons_text[i], text_delta_y);
            int text_x = (int) (buttons[i].x + (buttons[i].width - text_delta_x) / 2);
            int text_y = (int) (buttons[i].y + (buttons[i].height - text_delta_y) / 2);
            DrawText(buttons_text[i], text_x, text_y, text_delta_y, button_color);
            
        }

        EndDrawing();

    }

    return pressed_button + BUTTON_PLAY - 1;

}


void raylib_show_text(const char* maintext, int size = 35) {

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_ENTER)) {
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(maintext, 5, 5, size, GREEN);
        EndDrawing();

    }

    PollInputEvents();

    return;

}
