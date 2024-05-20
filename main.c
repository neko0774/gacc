#include "gacc.h"


int main(int argc, char **argv) {
    if (argc != 2) {
    	fprintf(stderr, "incorrect input\n");
    	return 1;
    }

  	//tokenize
    Token *tok = tokenize(argv[1]);
    Node *node = program(tok);

  	//begining of the assembly
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    //変数分の領域確保
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp,208\n");

    gen(node);

    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}
