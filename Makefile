CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

gacc: $(OBJS)
	$(CC) -o gacc $(OBJS) $(LDFLAGS)

$(OBJS): gacc.h

test: gacc
	./test.sh

clean:
	rm -f gacc *.o *~ tmp*

.PHONY: test clean
