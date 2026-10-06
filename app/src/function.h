#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "misc/lv_area.h"
#include "misc/lv_style.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define TOKEN_MAX_LENGTH 8
#define FUNCTION_NUM_POINTS 512

// Should end with a NULL.
#define FUNCTION_TOKEN_BUFFER(name, size) static char (name)[size][TOKEN_MAX_LENGTH]

#define __LEFT 'L'
#define __RIGHT 'R'

#define _M_PI 3.14159265358979323846
#define _M_E  2.7182818284590452354

/**
 * Contains information about a function, including all its points in
 * floating-point format, as well as the LVGL points to draw
 */
typedef struct Function {
  float x[FUNCTION_NUM_POINTS];
  float y[FUNCTION_NUM_POINTS];
  lv_point_precise_t points[FUNCTION_NUM_POINTS];
  lv_style_t style;
  bool is_active;
} Function;

/**
 * Different types of tokens that arre recognized by the function parser
 */
typedef enum TokenType {
  TOKEN_NONE,
  TOKEN_OPERATOR,
  TOKEN_FUNCTION,
  TOKEN_CONSTANT,
  TOKEN_LITERAL,
  TOKEN_PARENTHESIS,
  TOKEN_X
} TokenType;

/**
 * Operator attributes required for each operator when converting to postfix
 * notation
 */
typedef enum OperatorAttribute {
  OPERATOR_PRECEDENCE,
  OPERATOR_ASSOCIATIVITY
} OperatorAttribute;


/**
 * Describes a single operator in its required detail
 */
typedef struct Operator {
  char *symbol;
  int8_t precedence;
  char associativity;
} Operator;

/**
 * Describes constant values (e.g. pi)
 */
typedef struct Constant {
  char *symbol;
  float value;
} Constant;

/**
 * Describes a single token of an input infix string
 */
typedef struct Token {
  TokenType token_type;
  union {
    Operator operator;
    char *function;
    Constant constant;
  };
} Token;

/**
 * Performs a full infix to postfix function conversion, which must be ones
 * before it can be evaluated for a given x input.
 *
 * Returns a negative integer on failure.
 */
int8_t function_infix_to_postfix(char (*infix)[TOKEN_MAX_LENGTH], char (*postfix)[TOKEN_MAX_LENGTH], size_t token_buffer_size);

/**
 * Evaluates a given postfix function for a given input x value. Evaluates y,
 * and returns it.
 */
float function_evaluate_postfix(char (*postfix)[TOKEN_MAX_LENGTH], float x_val);

/*
* Operator Stack Components
*/
#define OPERATOR_STACK_CAPACITY 256

typedef struct OperatorStack {
  char data[OPERATOR_STACK_CAPACITY][TOKEN_MAX_LENGTH];
  int32_t top;
} OperatorStack;

#define OPERATOR_STACK_INIT(_name) static OperatorStack _name = { .top=-1 }

char *ostack_peek(OperatorStack *stack, char *dest);
char *ostack_pop(OperatorStack *stack, char* dest);
int8_t ostack_push(OperatorStack *stack, char *str);

#endif
