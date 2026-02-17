#ifndef STACKF_H_
#define STACKF_H_

#include <stdint.h>

#define STACKF_CAPACITY 256

typedef struct StackF {
  float data[STACKF_CAPACITY];
  int32_t top;
} StackF;

#define STACKF_INIT(_name) static StackF _name = { .top=-1 }

float stackf_peek(StackF *stack);
float stackf_pop(StackF *stack);
int8_t stackf_push(StackF *stack, float val);

#endif
