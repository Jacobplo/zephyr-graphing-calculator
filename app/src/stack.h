#ifndef STACK_H_
#define STACK_H_

#include <stdint.h>

typedef struct Stack {
  char **data;
  int32_t top;
  int32_t capacity;
} Stack;

#define STACK_INIT(_name, _data, _capacity) Stack _name = { .data=_data, .top=-1, .capacity=(_capacity) }

char *stack_peek(Stack *stack, char* dest);
char *stack_pop(Stack *stack, char* dest);
int8_t stack_push(Stack *stack, char *str);

#endif
