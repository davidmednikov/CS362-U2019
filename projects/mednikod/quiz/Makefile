CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o testme testme.c  $(CFLAGS)

default: testme

clean:
	rm -f *.o
