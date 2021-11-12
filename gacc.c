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
	int len;// length of token string
};

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

typedef struct Node Node;

//abstract syntax tree
//types of node
struct Node{
	NodeKind kind;
	Node *lhs;
	Node *rhs;
	int val;
};


//Token which are focused
Token *token;
char *user_input;

//create new node
Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

//create new node
//just a number
Node *new_node_num(int val) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
	return node;
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
	exit(1);
}

//if a next token is an expected sign, read a next token and reurn true
//else return false
bool consume(char *op){
	if (token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len))
		return false;
	token = token -> next;
	return true;
}

//if a next token is an expected sign, read a next token
//else raise an error
void expect(char *op) {
  if (token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len))
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

//create a new token and connect to cur
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	tok->len = len;
	cur->next = tok;
 	return tok;
}

bool check_parameter(char *a, char *b){
	if (memcmp(a, b, strlen(b))==0)
		return true;
	else
		return false;
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

		if (check_parameter(p, "==") || check_parameter(p, ">=") || check_parameter(p, "<=") || check_parameter(p, "!=")){
			cur = new_token(TK_RESERVED, cur, p, 2);
			p += 2;
			continue;
		}	

    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p=='(' || *p==')' || *p=='>' || *p=='<') {
      cur = new_token(TK_RESERVED, cur, p, 1);
			p++;
      continue;
    }

    if (isdigit(*p)) {
    cur = new_token(TK_NUM, cur, p, 0);
    cur->val = strtol(p, &p, 10);
    continue;
    }

    error_at(p, "cannot tokenize");
    }
	new_token(TK_EOF, cur, p, 0);
	return head.next;
}

//declareation of the functions
Node *mul();
Node *add();
Node *primary();
Node *expr();
Node *unary();
Node *equlity();
Node *relational();
Node *equality();

//expr       = equality
//equality   = relational ("==" relational | "!=" relational)*
//relational = add ("<" add | "<=" add | '>' add | ">=" add)*
//add        = mul ("+" mul | "-" mul)*
//mul        = unary ("*" unary | "/" unary)*
//unary      = ("+" | "-")? primary
//primary    = num | "(" expr ")"

//fucntions for calculation
Node *expr() {
	return equality();
}

Node *equality(){
	// if the token is == or !=,
	Node *node = relational();
	for (;;){
		if (consume("=="))
			node = new_node(ND_EQ, node, relational());
		else if (consume("!="))
			node = new_node(ND_NE, node, relational());
		else
			return node;
	}
}

Node *relational() {
	Node *node = add();
	for (;;){
		if (consume("<"))
			node = new_node(ND_LT, node, add());
		else if (consume("<="))
			node = new_node(ND_LEQ, node, add());
		else if (consume(">"))
			node = new_node(ND_LT, add(), node);
		else if (consume(">="))
			node = new_node(ND_LEQ, add(), node);
		else
			return node;
	}
}

Node *add() {
  	Node *node = mul();
  	for (;;) {
  		if (consume("+"))
      		node = new_node(ND_ADD, node, mul());
    	else if (consume("-"))
      		node = new_node(ND_SUB, node, mul());
    	else
      		return node;
  }
}

Node *mul() {
  	Node *node = unary();
  	for (;;) {
  		if (consume("*"))
      		node = new_node(ND_MUL, node, unary());
    	else if (consume("/"))
      		node = new_node(ND_DIV, node, unary());
    	else
      		return node;
  	}
}

Node *primary() {
  	// if the next token is "(" then there should be "(" expr ")"
  	if (consume("(")) {
  		Node *node = expr();
    	expect(")");
    	return node;
	}

  	//otherwise, there should be a number
  	return new_node_num(expect_number());
}

Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

void gen(Node *node) {
	if (node->kind == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  	}

	gen(node->lhs);
	gen(node->rhs);

	printf("  pop rdi\n");
	printf("  pop rax\n");

	switch (node->kind) {
		case ND_ADD:
		printf("  add rax, rdi\n");
		break;
	case ND_SUB:
		printf("  sub rax, rdi\n");
		break;
	case ND_MUL:
		printf("  imul rax, rdi\n");
		break;
	case ND_DIV:
		printf("  cqo\n");
		printf("  idiv rdi\n");
		break;
	case ND_EQ:
		printf("	cmp rax, rdi\n");
		printf("	sete al\n");
		printf("	movzb rax, al\n");
		break;
	case ND_NE:
		printf("  cmp rax, rdi\n");
		printf("  setne al\n");
		printf("  movzb rax, al\n");
		break;
	case ND_LT:
		printf("	cmp rax, rdi\n");
		printf("	setl al\n");
		printf("	movzb rax, al\n");
		break;
	case ND_LEQ:
		printf("	cmp rax, rdi\n");
		printf("	setle al\n");
		printf("	movzb rax, al\n");
		break;
	}

	printf("  push rax\n");
}

int main(int argc, char **argv) {
	    if (argc != 2) {
	        fprintf(stderr, "the number of input are incorrect\n");
	        return 1;
	    }

		user_input = argv[1];
		//tokenize
		token = tokenize(argv[1]);
		Node *node = expr();

		//begining of the assembly
	    printf(".intel_syntax noprefix\n");
	    printf(".globl main\n");
	    printf("main:\n");

		gen(node);

		printf("  pop rax\n");
		printf("  ret\n");
		return 0;
          
}
