#include "gacc.h"


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