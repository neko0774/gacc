#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>


typedef enum{
	TK_RESERVED, //sign
	TK_NUM, //number
	TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token{
	TokenKind kind; //kind of token
	Token *next; //next input token
	int val; // if kind if num, the number
	char *str; //token string
};

//Token focused
Token *token;

//report error
void error_at(char *loc, char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, " ");
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

//if a next token is an expected sign, read a next token and reurn true
//else return false
bool consume(char op){
	if (token->kind != TK_RESERVED | token->str[0] != op)
		return false;
	token = token -> next;
	return true;
}

//if a next token is an expected sign, read a next token
//else raise an error
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error_at(token->str, "not a number");
  token = token->next;
}

//if a next token is an expected sign, read a next token
//else raise an error
int expect_number(){
	if (token->kind != TK_NUM)
		error_at(token->str, "not a number");
	int val = token->val;
	token = token -> next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

//create a new token and connect to cur
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}
//tokenaize input p and return p
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // skip blank
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(token->str, "cannot tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
	    if (argc != 2) {
	        fprintf(stderr, "the number of input are incorrect\n");
	        return 1;
	    }

		char *user_input;
		user_input = argv[1];
		//tokenize
		token = tokenize(argv[1]);
		
		//begining of the assembly
	    printf(".intel_syntax noprefix\n");
	    printf(".globl main\n");
	    printf("main:\n");
		//begining of the code must be a number
	    printf("  mov rax, %d\n", expect_number());

		while (!at_eof()) {
			if (consume('+')) {
				printf("  add rax, %d\n", expect_number());
				continue;
			}

			expect('-');
			printf("  sub rax, %d\n", expect_number());
		}

		printf("  ret\n");
		return 0;
          
}
