FLAGS=-std=c11 -g -static

gacc: gacc.c

test: gacc
	        ./test.sh

clean:
	        rm -f gacc *.o *~ tmp*

.PHONY: test clean
