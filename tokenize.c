#include "gacc.h"


//create a new token and connect to cur
//cur is a current token which is focused
//cur is updated to tok as new a token is created
//then tok becomes cur
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	tok->len = len;
	cur->next = tok;
 	return tok;
}

bool check_parameter(char *a, char *b){
	if (memcmp(a, b, strlen(b))==0){
		return true;
  }else{
		return false;
  }
}

//tokenize input p and return p
void *tokenize(char *p) {
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {
		// skip blank
	  if (isspace(*p)) {
	  	p++;
	  	continue;
	  }

		if (check_parameter(p, "==") || check_parameter(p, ">=") || check_parameter(p, "<=") || check_parameter(p, "!=")){
			cur = new_token(TK_RESERVED, cur, p, 2);
			p += 2;
			continue;
		}	

	  if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p=='(' || *p==')' || *p=='>' || *p=='<' || *p==';') {
      cur = new_token(TK_RESERVED, cur, p, 1);
      p++;
	    continue;
	  }

		if ('a' <= *p && *p <= 'z'){
			cur = new_token(TK_INDENT, cur, p, 1);
			p++;
			continue;
		}

	  if (isdigit(*p)) {
	  	cur = new_token(TK_NUM, cur, p, 0);
      char *q = p;
	  	cur->val = strtol(p, &p, 10);
	  	continue;
	  }
	  error_at(p, "cannot tokenize");
	}
	new_token(TK_EOF, cur, p, 0);
	token = head.next;
}

void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

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
	fprintf(stderr, "%s and %d\n", token->str, token->kind);
	exit(1);
}

//if a next token is an expected sign, read a next token and reurn true
//else return false
bool consume(char *op){
	if (token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len)){
    return false;
  }
	token = token -> next;
	return true;
}

//if a next token is an expected sign, read a next token
//else raise an error
void expect(char *op) {
	if (token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len)){
		fprintf(stderr, "%d\n", token->kind);
  	error_at(token->str, "unexpected token used");
	}
	token = token->next;
}

//if a next token is an expected sign, read a next token
//else raise an error
int expect_number(){
	if (token->kind != TK_NUM) {
    error_at(token->str, "not a number");
  }
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

Token *consume_ident(){
	if (token->kind == TK_INDENT){
    return token;
  }
	return NULL;
}