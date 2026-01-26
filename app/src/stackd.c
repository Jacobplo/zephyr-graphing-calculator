#include "stackd.h"
#include <stdbool.h>

double stackd_peek(StackD *stack) {
  if(stack->top == -1) return 0;

  return stack->data[stack->top];
}

double stackd_pop(StackD *stack) {
  if(stack->top == -1) return 0;

  double ret = stack->data[stack->top];
  stack->top--;
  return ret;
}

int8_t stackd_push(StackD *stack, double val) {
  if(stack->top == STACKD_CAPACITY - 1) return 0;

  stack->top++;
  stack->data[stack->top] = val;

  return 1;

}

bool stackd_is_empty(StackD *stack) {
  return stack->top == -1 ? true : false;
}
