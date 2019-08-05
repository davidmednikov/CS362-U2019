CFLAGS= -Wall -fpic -coverage -lm -std=c99

SRCDIR= src
RNDDIR= random
UNTDIR= unit

rngs.o: $(SRCDIR)/rngs.h $(SRCDIR)/rngs.c
	gcc -c $(SRCDIR)/rngs.c -g  $(CFLAGS)

randomdominion.o: $(SRCDIR)/dominion.h randomdominion.c rngs.o
	gcc -o randomdominion.o -c randomdominion.c -g  $(CFLAGS)

unitdominion.o: $(SRCDIR)/dominion.h unitdominion.c rngs.o
	gcc -o unitdominion.o -c unitdominion.c -g  $(CFLAGS)

playdom: dominion.o $(SRCDIR)/playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: $(SRCDIR)/testDrawCard.c $(SRCDIR)/dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: $(SRCDIR)/badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: $(SRCDIR)/testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o $(SRCDIR)/testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: $(SRCDIR)/interface.h $(SRCDIR)/interface.c
	gcc -c interface.c -g  $(CFLAGS)

unittest1: $(UNTDIR)/unittest1.c unitdominion.o rngs.o
	gcc -o unittest1 -g $(UNTDIR)/unittest1.c unitdominion.o rngs.o $(CFLAGS)

unittest2: $(UNTDIR)/unittest2.c unitdominion.o rngs.o
	gcc -o unittest2 -g $(UNTDIR)/unittest2.c unitdominion.o rngs.o $(CFLAGS)

unittest3: $(UNTDIR)/unittest3.c unitdominion.o rngs.o
	gcc -o unittest3 -g $(UNTDIR)/unittest3.c unitdominion.o rngs.o $(CFLAGS)

unittest4: $(UNTDIR)/unittest4.c unitdominion.o rngs.o
	gcc -o unittest4 -g $(UNTDIR)/unittest4.c unitdominion.o rngs.o $(CFLAGS)

unittest5: $(UNTDIR)/unittest5.c unitdominion.o rngs.o
	gcc -o unittest5 -g $(UNTDIR)/unittest5.c unitdominion.o rngs.o $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittestresults: unittest1 unittest2 unittest3 unittest4 unittest5 unitdominion.o
	unittest1 > $(UNTDIR)/unittestresults.out
	unittest2 >> $(UNTDIR)/unittestresults.out
	unittest3 >> $(UNTDIR)/unittestresults.out
	unittest4 >> $(UNTDIR)/unittestresults.out
	unittest5 >> $(UNTDIR)/unittestresults.out
	gcov unitdominion.c -f -b >> $(UNTDIR)/unittestresults.out
	cat unitdominion.c.gcov >> $(UNTDIR)/unittestresults.out

randomtestcard1: $(RNDDIR)/randomtestcard1.c randomdominion.o rngs.o
	gcc -o randomtestcard1 -g $(RNDDIR)/randomtestcard1.c randomdominion.o rngs.o $(CFLAGS)

randomtestcard2: $(RNDDIR)/randomtestcard2.c randomdominion.o rngs.o
	gcc -o randomtestcard2 -g $(RNDDIR)/randomtestcard2.c randomdominion.o rngs.o $(CFLAGS)

randomtestcard3: $(RNDDIR)/randomtestcard3.c randomdominion.o rngs.o
	gcc -o randomtestcard3 -g $(RNDDIR)/randomtestcard3.c randomdominion.o rngs.o $(CFLAGS)

randomtestresults: randomtestcard1 randomtestcard2 randomtestcard3 randomdominion.o
	randomtestcard1 > $(RNDDIR)/randomtestresults.out
	randomtestcard2 >> $(RNDDIR)/randomtestresults.out
	randomtestcard3 >> $(RNDDIR)/randomtestresults.out
	gcov randomdominion.c -f -b >> $(RNDDIR)/randomtestresults.out
	cat randomdominion.c.gcov >> $(RNDDIR)/randomtestresults.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: randomdominion.o unitdominion.o randomtestresults unittestresults

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
