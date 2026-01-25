#ifndef STACK_H_
#define STACK_H_

#include "zephyr/kernel.h"
#include <stdint.h>

#define STACK_CAPACITY 1024

typedef struct Stack {
  char data[STACK_CAPACITY][16];
  int32_t top;
} Stack;

#define STACK_INIT(_name) static Stack _name = { .top=-1 }

char *stack_peek(Stack *stack, char *dest);
char *stack_pop(Stack *stack, char* dest);
int8_t stack_push(Stack *stack, char *str);

#endif
