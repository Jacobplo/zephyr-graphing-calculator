#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define TOKEN_MAX_LENGTH 16

// Should end with a NULL.
#define FUNCTION_TOKEN_BUFFER(name, size) static char (name)[size][TOKEN_MAX_LENGTH]

#define __LEFT 'L'
#define __RIGHT 'R'

#define _M_PI 3.14159265358979323846
#define _M_E  2.7182818284590452354


typedef struct Function {
  double x[512];
  double y[512];
} Function;


typedef enum TokenType {
  TOKEN_NONE,
  TOKEN_OPERATOR,
  TOKEN_FUNCTION,
  TOKEN_CONSTANT,
  TOKEN_LITERAL,
  TOKEN_PARENTHESIS,
  TOKEN_X
} TokenType;

typedef enum OperatorAttribute {
  OPERATOR_PRECEDENCE,
  OPERATOR_ASSOCIATIVITY
} OperatorAttribute;


typedef struct Operator {
  char *symbol;
  int8_t precedence;
  char associativity;
} Operator;

typedef struct Constant {
  char *symbol;
  double value;
} Constant;

typedef struct Token {
  TokenType token_type;
  union {
    Operator operator;
    char *function;
    Constant constant;
  };
} Token;


int8_t function_infix_to_postfix(char (*infix)[TOKEN_MAX_LENGTH], char (*postfix)[TOKEN_MAX_LENGTH], size_t token_buffer_size);
TokenType __function_get_token_type(const char *token);
double __function_get_constant(const char *token);
int8_t __function_get_operator_attribute(const char *token, OperatorAttribute attribute);



/*
* Operator Stack Components
*/
#define OPERATOR_STACK_CAPACITY 1024

typedef struct OperatorStack {
  char data[OPERATOR_STACK_CAPACITY][TOKEN_MAX_LENGTH];
  int32_t top;
} OperatorStack;

#define OPERATOR_STACK_INIT(_name) static OperatorStack _name = { .top=-1 }

char *stack_peek(OperatorStack *stack, char *dest);
char *stack_pop(OperatorStack *stack, char* dest);
int8_t stack_push(OperatorStack *stack, char *str);


#endif
