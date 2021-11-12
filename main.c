#include "gacc.h"

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
