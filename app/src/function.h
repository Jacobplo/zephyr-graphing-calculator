#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <stdbool.h>
#include <stdint.h>


#define FUNCTION_TOKEN_BUFFER(name, size) static char *(name)[size]

#define __LEFT 'L'
#define __RIGHT 'R'


typedef struct Function {
  double x[512];
  double y[512];
} Function;


typedef enum TokenType {
  TOKEN_OPERATOR,
  TOKEN_FUNCTION,
  TOKEN_CONSTANT,
  TOKEN_NONE
} TokenType;

typedef struct Operator {
  char *symbol;
  int8_t precedence;
  char associativity;
} Operator;

typedef struct Token {
  TokenType token_type;
  union {
    Operator operator;
    char *function;
    char *constant;
  };
} Token;


void function_infix_to_postfix(char **infix, char **postfix);
bool __function_is_valid_token(const char *token);

#endif
