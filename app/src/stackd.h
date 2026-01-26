#ifndef STACKD_H_
#define STACKD_H_

#include <stdint.h>

#define STACKD_CAPACITY 256

typedef struct StackD {
  double data[STACKD_CAPACITY];
  int32_t top;
} StackD;

#define STACKD_INIT(_name) static StackD _name = { .top=-1 }

double stack_peek(StackD *stack);
double stack_pop(StackD *stack);
int8_t stack_push(StackD *stack, double val);

#endif
