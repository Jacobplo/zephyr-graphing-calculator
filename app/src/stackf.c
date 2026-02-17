#include "stackf.h"
#include <stdbool.h>

double stackf_peek(StackF *stack) {
  if(stack->top == -1) return 0;

  return stack->data[stack->top];
}

double stackf_pop(StackF *stack) {
  if(stack->top == -1) return 0;

  double ret = stack->data[stack->top];
  stack->top--;
  return ret;
}

int8_t stackf_push(StackF *stack, float val) {
  if(stack->top == STACKF_CAPACITY - 1) return 0;

  stack->top++;
  stack->data[stack->top] = val;

  return 1;

}

bool stackf_is_empty(StackF *stack) {
  return stack->top == -1 ? true : false;
}
