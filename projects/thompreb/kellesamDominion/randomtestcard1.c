#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testFunctions.h"

#define TESTCARD "cutpurse"
#define NOISY_TEST 1


void expectedChange(struct gameState *T);


int main () {
	struct gameState G, T;

	printf("\n[ Testing card %s ]\n", TESTCARD);
	
	int failed = 0;
	
	int i;
	for(i = 0; i < 1000; i++) {
		printf("# Random Test #%i\n", i+1);
		
		// Randomize gameState
		// Smaller handCount to increase chance of player not having copper
		randomState(&G, rand() % 20, -1, -1);
		
		printf("State -- Players: %i Active Player: %i\n", G.numPlayers, G.whoseTurn);
		
		// Make a copy of new gameState
		memcpy(&T, &G, sizeof(struct gameState));
		
		// Perform card action
		//playCutpurse(&G, 0);
		cardEffect(cutpurse, 0, 0, 0, &G, 0, 0);
		
		expectedChange(&T);
		
		if(compareGameState(G, T)) {
			printf("- All subtests passed\n");
		}
		else {
			printf("- Some subtests failed\n");
			failed++;
		}

	}
	
	if(!failed) {
		printf("\n[ All tests passed! ]\n");
	}
	else {
		printf("\n[ %i out of %i tests failed. ]\n", failed, i);
	}
	
	return 0;
}

void expectedChange(struct gameState *T) {
	int curPlayer = T->whoseTurn;
	
	T->coins += 2;
	
	// Played card removed from hand
	T->handCount[curPlayer]--;
	
	T->playedCardCount++;
	
	int i;
	for(i = 0; i < T->numPlayers; i++) {
		// For all other players
		if(i != curPlayer) {
			// If they had a copper in hand
			if(cardsIn(T->hand[i], T->handCount[i], copper)) {
				// Lose a copper from hand
				T->handCount[i]--;
				
				// Copper goes to discard pile
				T->discardCount[i]++;
			}
		}
	}
}