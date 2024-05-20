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
	TK_INDENT, //identifier
	TK_NUM, //number
	TK_EOF, //EOF
} TokenKind;

struct Token{
	TokenKind kind; //kind of token
	Token *next; //next input token
	int val; // if kind if num, the number
	char *str; //token string
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

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool check_parameter(char *a, char *b);
void *tokenize(char *p);
Token *consume_ident();

//declareation of the functions
void program();

Node *mul();
Node *add();
Node *primary();
Node *expr();
Node *unary();
Node *equality();
Node *relational();
Node *equality();
Node *assign();
Node *stmt();

void gen(Node *node);

//Token which are focused
extern Token *token;
extern char *user_input;
extern Node *code[100];