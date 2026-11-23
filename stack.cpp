#include <stdio.h>
#include <assert.h>
#include <string.h>


typedef const char* type;
const type POIZON = NULL;
const size_t MIN_POP_CAP = 2;


#define SPECIFICATOR "%s"


enum stack_error {
    STACK_OK = 0,
    STACK_NO_MEMORY,
    STACK_NULLPTR,
    STACK_WRONG_VAL
};


struct stack_t {
    type* real_buffer;
    type* data;
    ssize_t size;
    ssize_t capacity;
};


void struct_null(stack_t* stack);
stack_error stack_constructor(stack_t* stack, ssize_t capacity);
stack_error stack_raise(stack_t* stack);
stack_error stack_push(stack_t* stack, type new_value);
stack_error stack_cut(stack_t* stack);
stack_error stack_pop(stack_t* stack, type* output_value);
stack_error stack_destructor(stack_t* stack);
void stack_dump(stack_t* stack);
bool stack_verify(stack_t* stack);
void stack_print(stack_t* stack);


void struct_null(stack_t* stack) {

    stack->real_buffer = NULL;
    stack->data = NULL;
    stack->size = 0;
    stack->capacity = 0;

}


stack_error stack_constructor(stack_t* stack, ssize_t capacity) {

    assert(stack);
    assert(stack->data == NULL);
    assert(stack->real_buffer == NULL);
    assert(capacity > 0);

    struct_null(stack);

    stack->real_buffer = (type*) calloc (capacity + 2, sizeof(type));

    if (stack->real_buffer) {

        stack->data = stack->real_buffer + 1;
        stack->capacity = capacity;
        stack->size = 0;

        for (ssize_t index = stack->size; index < capacity; index++) {
            (stack->data)[index] = POIZON;
        }

        return STACK_OK;

    } 
    else {

        return STACK_NO_MEMORY;

    }

}


stack_error stack_raise(stack_t* stack) {

    type* new_real_buffer = (type*) realloc(stack->real_buffer, (stack->capacity * 2) * sizeof(type) + 2 * sizeof(type));

    if (new_real_buffer != NULL) {
        stack->capacity = stack->capacity * 2;
        stack->real_buffer = new_real_buffer;
        stack->data = stack->real_buffer + 1;

        for (ssize_t index = stack->size; index < stack->capacity; index++) {
            (stack->data)[index] = POIZON;
        }

        return STACK_OK;

    } 
    else {

        return STACK_NULLPTR;
    
    }

}


stack_error stack_push(stack_t* stack, type new_value) {

    assert(stack_verify(stack));

    if (stack->capacity <= 0) {
        return STACK_WRONG_VAL;
    }

    if (stack->size >= stack->capacity) {

        if (stack_raise(stack) != STACK_OK) {
            return STACK_NULLPTR;
        }

    }

    *(stack->data + stack->size) = new_value;
    (stack->size)++;

    return STACK_OK;

}


stack_error stack_cut(stack_t* stack) {

    type* new_real_buffer = (type*) realloc(stack->real_buffer, (stack->capacity / 2) * sizeof(type) + 2 * sizeof(type));

    if (new_real_buffer == NULL) {

        return STACK_NULLPTR;

    } 
    else {

        stack->capacity = stack->capacity / 2;
        stack->real_buffer = new_real_buffer;
        stack->data = new_real_buffer + 1;

        return STACK_OK;
    
    }

}


stack_error stack_pop(stack_t* stack, type* output_value) {

    assert(stack_verify(stack));
    assert(output_value);

    stack_error is_error = STACK_OK;

    if (stack->size <= 0) {
        return STACK_WRONG_VAL;
    }

    *output_value = *(stack->data + stack->size - 1);
    *(stack->data + stack->size - 1) = POIZON;
    (stack->size)--;

    if ((stack->size <= stack->capacity / 4) && (stack->capacity >= MIN_POP_CAP)) {
        is_error = stack_cut(stack);
    }

    return is_error;
}


stack_error stack_destructor(stack_t* stack) {

    if (stack == NULL) {
        return STACK_NULLPTR;
    }

    if (stack->real_buffer) {
        memset(stack->real_buffer, '\0', (stack->capacity + 2) * sizeof(type));
        free(stack->real_buffer);
        stack->real_buffer = NULL;
    }

    memset(stack, '\0', sizeof(stack_t));
    free(stack);

    return STACK_OK;

}


void stack_dump(stack_t* stack) {

    printf("\n\n[DUMPING]");
    printf(" Main information:\n");
    printf(" \t  Stack: %p ; Capacity: %zd ; Size: %zd ; Data = %p \n", stack, stack->capacity, stack->size, stack->data);

    for (ssize_t index = 0; index < stack->size; index++) {
        printf("\n\t  data[%5zd] = " SPECIFICATOR, index, (stack->data)[index]);
    }

    for (ssize_t index = stack->size; index < stack->capacity; index++) {
        printf("\n\t  data[%5zd] = " SPECIFICATOR, index, (stack->data)[index]);

        // if (!identity((stack->data)[index], POIZON)) {
        if (stack->data[index] != POIZON) {
            printf("\t!!!");
        } 
        else {
            printf("\t*POISON");
        }

    }

    printf("\n\n");

}


bool stack_verify(stack_t* stack) {

    if (stack == NULL) {
        return false;
    }

    if (stack->real_buffer == NULL) {
        return false;
    }

    if (stack->data == NULL) {
        return false;
    }

    if (stack->data != stack->real_buffer + 1) {
        return false;
    }

    if (stack->capacity <= 0) {
        return false;
    }

    if (stack->size < 0) {
        return false;
    }

    if (stack->size > stack->capacity) {
        return false;
    }

    return true;

}


void stack_print(stack_t* stack) {

    printf("Stack:\n");

    for (ssize_t index = 0; index < stack->size; index++) {
        printf("[%*zd]\t", 5, index);
    }

    printf("\n");

    for (ssize_t index = 0; index < stack->size; index++) {
        printf(" " SPECIFICATOR " \t", (stack->data)[index]);
    }

    printf("\n\n");

}


