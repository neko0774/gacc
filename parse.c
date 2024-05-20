#include "gacc.h"


//program    = stmt*
//stmt 			 = expr ";"
//expr       = assign
//assign     = equality ("=" assign)?
//equality   = relational ("==" relational | "!=" relational)*
//relational = add ("<" add | "<=" add | ">" add | ">=" add)*
//add        = mul ("+" mul | "-" mul)*
//mul        = unary ("*" unary | "/" unary)*
//unary      = ("+" | "-")? primary
//primary    = num | "(" expr ")"
Node *program(Token *token);
Node *mul(Token *token);
Node *add(Token *token);
Node *primary(Token *token);
Node *expr(Token *token);
Node *unary(Token *token);
Node *equality(Token *token);
Node *relational(Token *token);
Node *equality(Token *token);
Node *assign(Token *token);
Node *stmt(Token *token);

Token *consume_ident(Token *token){
	if (token->kind == TK_IDENT){
    return token;
  }
	return NULL;
}


//if a next token is an expected sign, read a next token and reurn true
//else return false
bool consume(Token *token, char *op){
	if (token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len)){
    return false;
  }
	token = token -> next;
	return true;
}


//if a next token is an expected sign, read a next token
//else raise an error
void expect(Token *token, char *op) {
	if (token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len)){
  	error_at(token->str, "unexpected token used");
	}
	token = token->next;
}

bool at_eof(Token *token) {
	return token->kind == TK_EOF;
}

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
Node *new_node_num(Token *token, int val) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
	return node;
}

//functions for calculation
Node *program(Token *token) {
	Node *node = stmt(token);
	if(at_eof(token))
		error_at(token->str, "invalid tokenising occued");
	return node;
}


Node *expr(Token *token) {
	return assign(token);
}

Node *assign(Token *token) {
	Node *node = equality(token);
	if (consume(token,"="))
		node = new_node(ND_ASSIGN, node, assign(token));
	return node;
}

Node *stmt(Token *token) {
	Node *node = expr(token);
	expect(token, ";");
	return node;
}

Node *equality(Token *token){
	// if the token is == or !=,
	Node *node = relational(token);
	for (;;){
		if (consume(token, "=="))
			node = new_node(ND_EQ, node, relational(token));
		else if (consume(token, "!="))
			node = new_node(ND_NE, node, relational(token));
		else
			return node;
	}
}

Node *relational(Token *token) {
	Node *node = add(token);
	for (;;){
		if (consume(token, "<"))
			node = new_node(ND_LT, node, add(token));
		else if (consume(token, "<="))
			node = new_node(ND_LEQ, node, add(token));
		else if (consume(token, ">"))
			node = new_node(ND_LT, add(token), node);
		else if (consume(token, ">="))
			node = new_node(ND_LEQ, add(token), node);
		else
			return node;
	}
}

Node *add(Token *token) {
	Node *node = mul(token);
	for (;;) {
		if (consume(token, "+"))
  		  node = new_node(ND_ADD, node, mul(token));
   		else if (consume(token, "-"))
				node = new_node(ND_SUB, node, mul(token));
   		else
    	 	return node;
	}
}

Node *mul(Token *token) {
 	Node *node = unary(token);
 	for (;;) {
 		if (consume(token, "*"))
    		node = new_node(ND_MUL, node, unary(token));
   		else if (consume(token, "/"))
    		node = new_node(ND_DIV, node, unary(token));
   		else
    		return node;
 	}
}

Node *primary(Token *token) {
	// if the next token is "(" then there should be "(" expr ")"
	if (consume(token, "(")) {
 		Node *node = expr(token);
   	expect(token, ")");
   	return node;
	}
	//if there is a variable
	Token *tok = consume_ident(token);
	if (tok) {
		Node *node = calloc(1, sizeof(Node));
		node->kind = ND_LVAR;
		node->offset = (tok->str[0]-'a'+1)*8;
		return node;
	}
	//otherwise, there should be a number
 	return new_node_num(token, expect_number(token));
}

Node *unary(Token *token) {
	if (consume(token, "+"))
		return primary(token);
	if (consume(token, "-"))
		return new_node(ND_SUB, new_node_num(token, 0), primary(token));
	return primary(token);
}