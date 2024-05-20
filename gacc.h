#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>


//tokenize
typedef struct Token Token;

typedef enum{
	TK_RESERVED, //sign
	TK_IDENT, //identifier
	TK_NUM, //number
	TK_EOF, //EOF
} TokenKind;

struct Token{
	TokenKind kind; //kind of token
	Token *next; //next input token
	int val; // if kind is num, its value
	char *str; //token string
	char *loc; //its location
	int len;// length of token string
};

typedef struct Node Node;

//abstract syntax tree
//kinds of node
typedef enum{
	ND_ADD, //+
	ND_SUB, //-
	ND_MUL, //*
	ND_DIV, // /
	ND_NUM, // number
	ND_EQ, // ==
	ND_NE, // !=
	ND_LEQ, // <=
	ND_LT, // <
	ND_ASSIGN, //=
	ND_LVAR, // local variable
} NodeKind;

//abstract syntax tree
//types of node
struct Node{
	NodeKind kind;
	Node *lhs;
	Node *rhs;
	int val;
	int offset;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
int expect_number();
Token *tokenize(char *p);

Node *program(Token *tok);
void gen(Node *node);