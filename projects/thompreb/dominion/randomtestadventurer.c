#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testFunctions.h"

#define TESTCARD "adventurer"

void expectedChange(struct gameState*);

int main () {
	struct gameState G, T;

	printf("\n[ Testing card %s ]\n", TESTCARD);
	
	int failed = 0;
	
	int i;
	for(i = 0; i < 100; i++) {
		printf("# Random Test #%i\n", i+1);
		
		// Randomize gameState
		// Deck and discard are smaller to give higher chance of 0 and 1 treasure piles
		randomState(&G, -1, rand() % 10, rand() % 10);
		
		// randomly place an adventurer card in current player's hand
		int handPos;
		if(G.handCount[G.whoseTurn] == 0) {
			handPos = 0;
		}
		else {
			handPos = rand() % G.handCount[G.whoseTurn];
		}
		G.hand[G.whoseTurn][handPos] = adventurer;
		
		// adding in a guaranteed treasure to prevent segfault
		//G.discard[G.whoseTurn][G.discardCount[G.whoseTurn]++] = copper;
		
		printf("State -- Players: %i Active Player: %i\n", G.numPlayers, G.whoseTurn);
		
		// Make a copy of new gameState
		memcpy(&T, &G, sizeof(struct gameState));
		
		// Perform card action
		cardEffect(adventurer, 0, 0, 0, &G, handPos, 0);
		
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
	int deckTreasure = cardsIn(T->deck[curPlayer], T->deckCount[curPlayer], copper) +
		cardsIn(T->deck[curPlayer], T->deckCount[curPlayer], silver) +
		cardsIn(T->deck[curPlayer], T->deckCount[curPlayer], gold);
	int discardTreasure = cardsIn(T->discard[curPlayer], T->discardCount[curPlayer], copper) +
		cardsIn(T->discard[curPlayer], T->discardCount[curPlayer], silver) +
		cardsIn(T->discard[curPlayer], T->discardCount[curPlayer], gold);
		
	// Insufficient treasure in deck
	if(deckTreasure + discardTreasure == 0) {
		// Player loses a card and gains none
		T->handCount[curPlayer] -= 1;
	}
	else if(deckTreasure + discardTreasure == 1) {
		// New coin total
		updateCoins(curPlayer, T, 0);
		
		// No change to hand (gain 1 discard 1)
	}
	else {
		// New coin total
		updateCoins(curPlayer, T, 0);
		
		// Player should gain 1 card (2 drawn 1 discarded)
		T->handCount[curPlayer] += 1;	
	}
	
	T->playedCardCount++;
}