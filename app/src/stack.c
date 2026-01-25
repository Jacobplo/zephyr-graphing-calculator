#include <string.h>

#include "stack.h"
#include "zephyr/kernel.h"
#include "zephyr/sys/printk.h"

char *stack_peek(Stack *stack, char *dest) {
  if(stack->top == -1) return NULL;

  return strcpy(dest, stack->data[stack->top]);
}

char *stack_pop(Stack *stack, char *dest) {
  if(stack->top == -1) return NULL;

  char *ret = strcpy(dest, stack->data[stack->top]);
  stack->top--;

  return ret;
}

int8_t stack_push(Stack *stack, char *str) {
  if(stack->top == STACK_CAPACITY - 1) return 0;

  stack->top++;

  strcpy(stack->data[stack->top], str);

  return 1;
}

