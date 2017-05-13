#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testFunctions.h"

#define TESTCARD "cutpurse"
#define NOISY_TEST 1

int main () {
	struct gameState G, T;

	printf("\n[ Testing card %s ]\n", TESTCARD);
	
	int failed = 0;
	
	int i;
	for(i = 0; i < 100; i++) {
		if(NOISY_TEST) {
			printf("# Random Test #%i\n", i+1);
		}
		
		// Randomize gameState
		randomState(&G, rand() % 50, -1, -1);
		
		// Make a copy of new gameState
		memcpy(&T, &G, sizeof(struct gameState));
		
		// Perform card action
		playCutpurse(&G, 0);
		
		// Conditions for current player and gamestate
		if(NOISY_TEST) {
			printf("- coins increases by 2\n");
		}
		assertTest(T.coins + 2, G.coins);
		
		if(NOISY_TEST) {
			printf("- Played cards increases by 1\n");
		}
		assertTest(T.playedCardCount + 1, G.playedCardCount);
		
		if(NOISY_TEST) {
			printf("- Current player hand decreases by 1\n");
		}
		assertTest(T.handCount[T.whoseTurn] - 1, G.handCount[G.whoseTurn]);
		
		// Conditions for other players
		int j;
		for(j = 0; j < G.numPlayers; j++) {
			if(j != G.whoseTurn) {
				// If player had a copper
				int initCopper = cardsIn(T.hand[j], T.handCount[j], copper);
				if(initCopper > 0) {
					printf("- Player %i one fewer card in hand\n", j);
					assertTest(T.handCount[j] - 1, G.handCount[j]);
					
					printf("- Player %i lost a copper from hand\n", j);
					assertTest(initCopper - 1, cardsIn(G.hand[j], G.handCount[j], copper));
					
					printf("- Player %i card was added to discard\n", j);
					assertTest(T.discardCount[i] + 1, G.discardCount[i]);
				}
			}
		}
	}
	
	if(!failed) {
		printf("\n[ All tests passed! ]\n");
	}
	else {
		printf("\n[ %i tests failed. ]\n", failed);
	}
	
	return 0;
}