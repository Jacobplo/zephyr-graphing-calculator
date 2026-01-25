#include "function.h"

#include <zephyr/kernel.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define IS_OPERATOR(str) (__function_get_token_type(str) == TOKEN_OPERATOR)
#define IS_FUNCTION(str) (__function_get_token_type(str) == TOKEN_FUNCTION)
#define IS_LEFT_PARENTHESIS(str) (!strcmp((str), "("))
#define GET_PRECEDENCE(str) (__function_get_operator_attribute((str), OPERATOR_PRECEDENCE))
#define GET_ASSOCIATIVITY(str) (__function_get_operator_attribute((str), OPERATOR_ASSOCIATIVITY))

static const Token possible_tokens[] = {
  // Operators
  { .token_type=TOKEN_OPERATOR, .operator={ "+", 2, __LEFT } },
  { .token_type=TOKEN_OPERATOR, .operator={ "-", 2, __LEFT } },
  { .token_type=TOKEN_OPERATOR, .operator={ "*", 3, __LEFT } },
  { .token_type=TOKEN_OPERATOR, .operator={ "/", 3, __LEFT } },
  { .token_type=TOKEN_OPERATOR, .operator={ "^", 4, __RIGHT } },

  // Functions
  { .token_type=TOKEN_FUNCTION, .function="sin" },
  { .token_type=TOKEN_FUNCTION, .function="cos" },
  { .token_type=TOKEN_FUNCTION, .function="tan" },
  { .token_type=TOKEN_FUNCTION, .function="abs" },
  { .token_type=TOKEN_FUNCTION, .function="sqrt" },
  { .token_type=TOKEN_FUNCTION, .function="ln" },
  { .token_type=TOKEN_FUNCTION, .function="floor" },
  { .token_type=TOKEN_FUNCTION, .function="ceil" },

  // Constants
  { .token_type=TOKEN_CONSTANT, .constant={ "pi",  _M_PI } },
  { .token_type=TOKEN_CONSTANT, .constant={ "e",  _M_E } },

  // Used to indicate searching for token should stop.
  { .token_type=TOKEN_NONE },
};



int8_t function_infix_to_postfix(char (*infix)[TOKEN_MAX_LENGTH], char (*postfix)[TOKEN_MAX_LENGTH], size_t token_buffer_size) {
  OPERATOR_STACK_INIT(operator_stack);

  char operator_stack_element_buffer[TOKEN_MAX_LENGTH];

  size_t i = 0;
  // Loops through all tokens in infix form, converting it to postfix form
  while((*infix)[0] != '\0' && i < token_buffer_size - 1) {
    char *token = *infix;
    TokenType token_type = __function_get_token_type(token);

    if(token_type == TOKEN_NONE) return 0;  // Fails if token is not a valid type.

    // Case for if token is a double literal or constant.
    if(token_type == TOKEN_LITERAL || token_type == TOKEN_CONSTANT) {
      strcpy(*postfix, token);
      postfix++;
    }

    // Case for if token is a function.
    else if(token_type == TOKEN_FUNCTION) {
      stack_push(&operator_stack, token); 
    }

    // Case for if token is an operator.
    else if(token_type == TOKEN_OPERATOR) {
      while(stack_peek(&operator_stack, operator_stack_element_buffer) != NULL &&
            IS_OPERATOR(operator_stack_element_buffer) && 
            !IS_LEFT_PARENTHESIS(operator_stack_element_buffer) &&
            (GET_PRECEDENCE(operator_stack_element_buffer) > GET_PRECEDENCE(token) ||
             (GET_PRECEDENCE(operator_stack_element_buffer) == GET_PRECEDENCE(token) &&
              GET_ASSOCIATIVITY(token) == __LEFT))) 
      {
        stack_pop(&operator_stack, *postfix);
        postfix++;
      }
      stack_push(&operator_stack, token);
    }

    // Case for if token is a left parenthesis.
    else if(token_type == TOKEN_PARENTHESIS && !strcmp(token, "(")) {
      stack_push(&operator_stack, token);
    }

    // Case for if token is a right parenthesis.
    else if(token_type == TOKEN_PARENTHESIS && !strcmp(token, ")")) {
      while(stack_peek(&operator_stack, operator_stack_element_buffer) != NULL &&
            strcmp(operator_stack_element_buffer, "("))
      {
        // Mismatched parentheses
        if(stack_peek(&operator_stack, operator_stack_element_buffer) == NULL) {
          return 0;
        }
        stack_pop(&operator_stack, *postfix);
        postfix++;
      }
      // Mismatched parentheses
      if(stack_peek(&operator_stack, operator_stack_element_buffer) != NULL &&
         strcmp(operator_stack_element_buffer, "("))
      {
          return 0;
      } 
      stack_pop(&operator_stack, operator_stack_element_buffer); // Discard left parentheses.
      if(stack_peek(&operator_stack, operator_stack_element_buffer) != NULL &&
         IS_FUNCTION(operator_stack_element_buffer))
      {
        stack_pop(&operator_stack, *postfix);
        postfix++;
      }
    }

    infix++;
    i++;
  } 

  // Pop the remaining items from the operator stack to the postfix queue.
  while(stack_peek(&operator_stack, operator_stack_element_buffer) != NULL) {
    // Mismatched parentheses.
    if(!strcmp(operator_stack_element_buffer, "(")) {
      return 0;
    }
    
    stack_pop(&operator_stack, *postfix);

    postfix++;
  }
 
  // Ensures the final token of each postfix expression is known.
  (*postfix)[0] = '\0';
  
  return 1;
}

TokenType __function_get_token_type(const char *token) { 
  // Compare token with the defined token types
  const Token *possible_token = possible_tokens;
  while(possible_token->token_type != TOKEN_NONE) { 
    if(possible_token->token_type == TOKEN_OPERATOR && !strcmp(possible_token->operator.symbol, token)) {
      return TOKEN_OPERATOR;
    }
    else if(possible_token->token_type == TOKEN_FUNCTION && !strcmp(possible_token->function, token)) {
      return TOKEN_FUNCTION;
    }
    else if(possible_token->token_type == TOKEN_CONSTANT && !strcmp(possible_token->constant.symbol, token)) {
      return TOKEN_CONSTANT;
    }

    possible_token++;
  } 

  // Check if token is a double literal.
  char *endptr;
  strtod(token, &endptr);
  if(endptr != token) return TOKEN_LITERAL;

  // Check if TOKEN is a symbol of none of the above types.
  if(strlen(token) == 1) {
    char token_chr = token[0];
    if(token_chr == 'x') return TOKEN_X;
    else if(token_chr == '(' || token_chr == ')') return TOKEN_PARENTHESIS;
  }


  return TOKEN_NONE;
}

double __function_get_constant(const char *token) {
  const Token *possible_token = possible_tokens;
  while(possible_tokens->token_type != TOKEN_NONE) {
    if(possible_token->token_type == TOKEN_CONSTANT && !strcmp(possible_token->constant.symbol, token)) {
      return possible_token->constant.value;
    }

    possible_token++;
  }

  return 0.0;
}

int8_t __function_get_operator_attribute(const char *token, OperatorAttribute attribute) {
  const Token *possible_token = possible_tokens;
  while(possible_tokens->token_type != TOKEN_NONE) {
    if(possible_token->token_type == TOKEN_OPERATOR && !strcmp(possible_token->operator.symbol, token)) {
      if(attribute == OPERATOR_PRECEDENCE) {
        return possible_token->operator.precedence;
      }
      else if(attribute == OPERATOR_ASSOCIATIVITY) {
        return possible_token->operator.associativity;
      }
    }

    possible_token++;
  }

  return 0;
}

/*
* Operator Stack Components
*/
char *stack_peek(OperatorStack *stack, char *dest) {
  if(stack->top == -1) return NULL;

  return strcpy(dest, stack->data[stack->top]);
}

char *stack_pop(OperatorStack *stack, char *dest) {
  if(stack->top == -1) return NULL;

  char *ret = strcpy(dest, stack->data[stack->top]);
  stack->top--;

  return ret;
}

int8_t stack_push(OperatorStack *stack, char *str) {
  if(stack->top == OPERATOR_STACK_CAPACITY - 1) return 0;

  stack->top++;

  strcpy(stack->data[stack->top], str);

  return 1;
}
