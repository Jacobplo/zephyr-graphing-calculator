#include "zephyr/sys/printk.h"

#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "function.h"


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
  { .token_type=TOKEN_CONSTANT, .constant="pi" },
  { .token_type=TOKEN_CONSTANT, .constant="e" },

  // Used to indicate searching for token should stop.
  { .token_type=TOKEN_NONE },
};



void function_infix_to_postfix(char **infix, char **postfix) {
  return;
}

bool __function_is_valid_token(const char *token) {
  const Token *possible_token = possible_tokens;
  while(possible_tokens->token_type != TOKEN_NONE) {
    if(possible_token->token_type == TOKEN_OPERATOR && !strcmp(possible_token->operator.symbol, token)) {
      return true;
    }
    else if(possible_token->token_type == TOKEN_FUNCTION && !strcmp(possible_token->function, token)) {
      return true;
    }
    else if(possible_token->token_type == TOKEN_CONSTANT && !strcmp(possible_token->constant, token)) {
      return true;
    }

    possible_token++;
  } 

  return false;
}
