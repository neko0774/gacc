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
	TK_NUM, //number
	TK_EOF,
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
	ND_ADD,
	ND_SUB,
	ND_MUL,
	ND_DIV,
	ND_NUM,
	ND_EQ,
	ND_NE,
	ND_LEQ,
	ND_LT,
} NodeKind;

//abstract syntax tree
//types of node
struct Node{
	NodeKind kind;
	Node *lhs;
	Node *rhs;
	int val;
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool check_parameter(char *a, char *b);
Token *tokenize(char *p);


//declareation of the functions
Node *mul();
Node *add();
Node *primary();
Node *expr();
Node *unary();
Node *equlity();
Node *relational();
Node *equality();

void gen(Node *node);

//Token which are focused
Token *token;
char *user_input;