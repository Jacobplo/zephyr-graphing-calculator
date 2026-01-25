#ifndef STACK_H_
#define STACK_H_

#include "zephyr/kernel.h"
#include <stdint.h>

typedef struct Stack {
  char **data;
  int32_t top;
  int32_t capacity;
} Stack;

#define STACK_INIT(_name, _data, _capacity) Stack _name = { .data=_data, .top=-1, .capacity=(_capacity) }

char *stack_peek(Stack *stack, char** dest, struct k_heap *heap);
char *stack_pop(Stack *stack, char** dest, struct k_heap *heap);
int8_t stack_push(Stack *stack, char *str, struct k_heap *heap);

#endif
