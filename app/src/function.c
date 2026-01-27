#include "function.h"

#include <zephyr/kernel.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stackd.h"


#define IS_OPERATOR(str) (__function_get_token_type(str) == TOKEN_OPERATOR)
#define IS_FUNCTION(str) (__function_get_token_type(str) == TOKEN_FUNCTION)
#define IS_LEFT_PARENTHESIS(str) (!strncmp((str), "(", TOKEN_MAX_LENGTH - 1))
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
    if(token_type == TOKEN_LITERAL || token_type == TOKEN_CONSTANT || token_type == TOKEN_X) {
      strncpy(*postfix, token, TOKEN_MAX_LENGTH - 1);
      postfix++;
    }

    // Case for if token is a function.
    else if(token_type == TOKEN_FUNCTION) {
      ostack_push(&operator_stack, token); 
    }

    // Case for if token is an operator.
    else if(token_type == TOKEN_OPERATOR) {
      while(ostack_peek(&operator_stack, operator_stack_element_buffer) != NULL &&
            IS_OPERATOR(operator_stack_element_buffer) && 
            !IS_LEFT_PARENTHESIS(operator_stack_element_buffer) &&
            (GET_PRECEDENCE(operator_stack_element_buffer) > GET_PRECEDENCE(token) ||
             (GET_PRECEDENCE(operator_stack_element_buffer) == GET_PRECEDENCE(token) &&
              GET_ASSOCIATIVITY(token) == __LEFT))) 
      {
        ostack_pop(&operator_stack, *postfix);
        postfix++;
      }
      ostack_push(&operator_stack, token);
    }

    // Case for if token is a left parenthesis.
    else if(token_type == TOKEN_PARENTHESIS && !strncmp(token, "(", TOKEN_MAX_LENGTH - 1)) {
      ostack_push(&operator_stack, token);
    }

    // Case for if token is a right parenthesis.
    else if(token_type == TOKEN_PARENTHESIS && !strncmp(token, ")", TOKEN_MAX_LENGTH - 1)) {
      while(ostack_peek(&operator_stack, operator_stack_element_buffer) != NULL &&
            strncmp(operator_stack_element_buffer, "(", TOKEN_MAX_LENGTH - 1))
      {
        // Mismatched parentheses
        if(ostack_peek(&operator_stack, operator_stack_element_buffer) == NULL) {
          return 0;
        }
        ostack_pop(&operator_stack, *postfix);
        postfix++;
      }
      // Mismatched parentheses
      if(ostack_peek(&operator_stack, operator_stack_element_buffer) != NULL &&
         strncmp(operator_stack_element_buffer, "(", TOKEN_MAX_LENGTH - 1))
      {
          return 0;
      } 
      ostack_pop(&operator_stack, operator_stack_element_buffer); // Discard left parentheses.
      if(ostack_peek(&operator_stack, operator_stack_element_buffer) != NULL &&
         IS_FUNCTION(operator_stack_element_buffer))
      {
        ostack_pop(&operator_stack, *postfix);
        postfix++;
      }
    }

    infix++;
    i++;
  } 

  // Pop the remaining items from the operator stack to the postfix queue.
  while(ostack_peek(&operator_stack, operator_stack_element_buffer) != NULL) {
    // Mismatched parentheses.
    if(!strncmp(operator_stack_element_buffer, "(", TOKEN_MAX_LENGTH - 1)) {
      return 0;
    }
    
    ostack_pop(&operator_stack, *postfix);

    postfix++;
  }
 
  // Ensures the final token of each postfix expression is known.
  (*postfix)[0] = '\0';
  
  return 1;
}


double function_evaluate_postfix(char (*postfix)[TOKEN_MAX_LENGTH], double x_val) {
  STACKD_INIT(operand_stack);

  while(*postfix[0]) {
    char *token = *postfix;
    TokenType token_type = __function_get_token_type(token);

    // Pushing any numbers, or x, to the operand stack.
    if(token_type == TOKEN_LITERAL) {
      stackd_push(&operand_stack, strtod(token, NULL));
    }
    else if(token_type == TOKEN_CONSTANT) {
      stackd_push(&operand_stack, __function_get_constant(token));
    }
    else if(token_type == TOKEN_X) {
      stackd_push(&operand_stack, x_val);
    }

    // Apply a unary function to the next operand on the stack.
    else if(token_type == TOKEN_FUNCTION) {
      if(!strncmp(token, "sin", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, sin(stackd_pop(&operand_stack)));
      }
      else if(!strncmp(token, "cos", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, cos(stackd_pop(&operand_stack)));
      }
      else if(!strncmp(token, "tan", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, tan(stackd_pop(&operand_stack)));
      }
      else if(!strncmp(token, "abs", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, fabs(stackd_pop(&operand_stack)));
      }
      else if(!strncmp(token, "sqrt", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, sqrt(stackd_pop(&operand_stack)));
      }
      else if(!strncmp(token, "ln", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, log(stackd_pop(&operand_stack)));
      }
      else if(!strncmp(token, "floor", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, floor(stackd_pop(&operand_stack)));
      }
      else if(!strncmp(token, "ceil", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, ceil(stackd_pop(&operand_stack)));
      }
    }

    // Apply binary operators to the next two operands on the stack.
    else if(token_type == TOKEN_OPERATOR) {
      double operand_one = stackd_pop(&operand_stack);
      double operand_two = stackd_pop(&operand_stack);
      if(!strncmp(token, "+", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, operand_two + operand_one);
      }
      else if(!strncmp(token, "-", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, operand_two - operand_one);
      }
      else if(!strncmp(token, "*", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, operand_two * operand_one);
      }
      else if(!strncmp(token, "/", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, operand_two / operand_one);
      }
      else if(!strncmp(token, "^", TOKEN_MAX_LENGTH - 1)) {
        stackd_push(&operand_stack, pow(operand_two, operand_one));
      }
    }

    postfix++;
  }
  
  return stackd_pop(&operand_stack);
}


TokenType __function_get_token_type(const char *token) { 
  // Compare token with the defined token types
  const Token *possible_token = possible_tokens;
  while(possible_token->token_type != TOKEN_NONE) { 
    if(possible_token->token_type == TOKEN_OPERATOR && !strncmp(possible_token->operator.symbol, token, TOKEN_MAX_LENGTH - 1)) {
      return TOKEN_OPERATOR;
    }
    else if(possible_token->token_type == TOKEN_FUNCTION && !strncmp(possible_token->function, token, TOKEN_MAX_LENGTH - 1)) {
      return TOKEN_FUNCTION;
    }
    else if(possible_token->token_type == TOKEN_CONSTANT && !strncmp(possible_token->constant.symbol, token, TOKEN_MAX_LENGTH - 1)) {
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
    if(possible_token->token_type == TOKEN_CONSTANT && !strncmp(possible_token->constant.symbol, token, TOKEN_MAX_LENGTH - 1)) {
      return possible_token->constant.value;
    }

    possible_token++;
  }

  return 0.0;
}

int8_t __function_get_operator_attribute(const char *token, OperatorAttribute attribute) {
  const Token *possible_token = possible_tokens;
  while(possible_tokens->token_type != TOKEN_NONE) {
    if(possible_token->token_type == TOKEN_OPERATOR && !strncmp(possible_token->operator.symbol, token, TOKEN_MAX_LENGTH - 1)) {
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
char *ostack_peek(OperatorStack *stack, char *dest) {
  if(stack->top == -1) return NULL;

  return strncpy(dest, stack->data[stack->top], TOKEN_MAX_LENGTH - 1);
}

char *ostack_pop(OperatorStack *stack, char *dest) {
  if(stack->top == -1) return NULL;

  char *ret = strncpy(dest, stack->data[stack->top], TOKEN_MAX_LENGTH - 1);
  stack->top--;

  return ret;
}

int8_t ostack_push(OperatorStack *stack, char *str) {
  if(stack->top == OPERATOR_STACK_CAPACITY - 1) return 0;

  stack->top++;

  strncpy(stack->data[stack->top], str, TOKEN_MAX_LENGTH - 1);

  return 1;
}
