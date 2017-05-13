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
	
	int i;
	for(i = 0; i < 100; i++) {
		printf("# Random Test #%i\n", i+1);
		
		// Randomize gameState
		// Deck and discard are smaller to give higher chance of 0 and 1 treasure piles
		randomState(&G, -1, rand() % 10, rand() % 10);
		
		// Make a copy of new gameState
		memcpy(&T, &G, sizeof(struct gameState));
		
		// Perform card action
		playAdventurer(&G);
		
		expectedChange(&T);
		
		if(compareGameState(G, T)) {
			printf("- All subtests passed\n");
		}
		else {
			printf("- Some subtests failed\n");
		}
		
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