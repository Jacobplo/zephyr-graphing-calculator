#include <string.h>

#include "stack.h"
#include "zephyr/kernel.h"
#include "zephyr/sys/printk.h"

char *stack_peek(Stack *stack, char **dest, struct k_heap *heap) {
  if(stack->top == -1) return NULL;

  *dest = k_heap_alloc(heap, strlen(stack->data[stack->top]), K_FOREVER);
  return strcpy(*dest, stack->data[stack->top]);
}

char *stack_pop(Stack *stack, char **dest, struct k_heap *heap) {
  if(stack->top == -1) return NULL;

  *dest = k_heap_alloc(heap, strlen(stack->data[stack->top]), K_FOREVER);

  char *ret = strcpy(*dest, stack->data[stack->top]);
  stack->top--;

  return ret;
}

int8_t stack_push(Stack *stack, char *str, struct k_heap *heap) {
  if(stack->top == stack->capacity - 1) return 0;

  stack->top++;

  stack->data[stack->top] = k_heap_alloc(heap, strlen(str), K_FOREVER);
  strcpy(stack->data[stack->top], str);

  return 1;
}

