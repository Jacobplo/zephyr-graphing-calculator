#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <stdbool.h>
#include <stdint.h>


// Should end with a NULL.
#define FUNCTION_TOKEN_BUFFER(name, size) static char *(name)[size]

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


int8_t function_infix_to_postfix(char **infix, char **postfix, int32_t buffer_size);
TokenType __function_get_token_type(const char *token);
double __function_get_constant(const char *token);
int8_t __function_get_operator_attribute(const char *token, OperatorAttribute attribute);

#endif
