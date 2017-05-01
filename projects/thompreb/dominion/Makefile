CFLAGS = -Wall -fpic -coverage -lm -std=c99
UNIT = unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4
UNIT_C = unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c
UNIT_O = unittest1.o unittest2.o unittest3.o unittest4.o cardtest1.o cardtest2.o cardtest3.o cardtest4.o

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

test: $(UNIT_C) dominion.o rngs.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)

unittestresults.out: $(UNIT_C) dominion.o rngs.o
	rm -f unittestresults.out
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)
	echo -e "########### Running unittest1.c ###########" >> unittestresults.out
	unittest1 >> unittestresults.out
	echo -e "\n########### Running unittest2.c ###########" >> unittestresults.out
	unittest2 >> unittestresults.out
	echo -e "\n########### Running unittest3.c ###########" >> unittestresults.out
	unittest3 >> unittestresults.out
	echo -e "\n########### Running unittest4.c ###########" >> unittestresults.out
	unittest4 >> unittestresults.out
	echo -e "\n########### Running cardtest1.c ###########" >> unittestresults.out
	cardtest1 >> unittestresults.out
	echo -e "\n########### Running cardtest2.c ###########" >> unittestresults.out
	cardtest2 >> unittestresults.out
	echo -e "\n########### Running cardtest3.c ###########" >> unittestresults.out
	cardtest3 >> unittestresults.out
	echo -e "\n########### Running cardtest4.c ###########" >> unittestresults.out
	cardtest4 >> unittestresults.out
	echo -e "\n########### Running GCOV on Dominion.c ###########\n" >> unittestresults.out
	gcov -f -b dominion.o >> unittestresults.out


testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom test.exe test player player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so $(UNIT)
