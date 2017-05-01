#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 0

#define TESTCARD "adventurer"

int assertTest(int expected, int observed);
void resetGamestate(struct gameState *G);
int testAdventurer(struct gameState *G);

int main() {
	struct gameState G;
	
	resetGamestate(&G);
	
	printf("\n[ Testing card %s ]\n", TESTCARD);
	
	int curPlayer = 0, passed = 1;
	printf("Current player = player %i\n", curPlayer);
	
	int i;
	
	// Player with 2 treasure in deck
	printf("\n# Nonempty Deck With Exactly 2 Treasure #\n");
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	G.deck[curPlayer][2] = copper;
	G.deck[curPlayer][4] = silver;
	if(!assertTest(1, testAdventurer(&G))) {
		passed = 0;
	}
	
	// Player with 3 treasure in deck
	printf("\n# Nonempty Deck With 3 Treasure #\n");
	resetGamestate(&G);
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	G.deck[curPlayer][1] = gold;
	G.deck[curPlayer][2] = copper;
	G.deck[curPlayer][4] = silver;
	if(!assertTest(1, testAdventurer(&G))) {
		passed = 0;
	}
	
	// Player with 1 treasure in deck and 2 in discard
	printf("\n# Must Shuffle Discard into Deck For Treasure #\n");
	resetGamestate(&G);
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	G.deck[curPlayer][2] = gold;
	G.discard[curPlayer][G.discardCount[curPlayer]++] = silver;
	G.discard[curPlayer][G.discardCount[curPlayer]++] = silver;
	if(!assertTest(1, testAdventurer(&G))) {
		passed = 0;
	}
	
	// Player has no treasure in deck or discard
	printf("\n# No treasure in deck or discard #\n");
	resetGamestate(&G);
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
		G.discard[curPlayer][G.discardCount[curPlayer]++] = village;
	}
	if(!assertTest(1, testAdventurer(&G))) {
		passed = 0;
	}
	
	// Results
	if(passed) {
		printf("\n[ All tests passed! ]\n");
	}
	else {
		printf("\n[ Not all tests passed. ]\n");
	}
	
	return 0;
}


int testAdventurer(struct gameState *G) {
	struct gameState T;
	int i;
	
	int curPlayer = 0;
	int cardsDrawn = 2;
	int discarded = 1;
	
	memcpy(&T, G, sizeof(struct gameState));
	
	// Testing variables
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	cardEffect(adventurer, choice1, choice2, choice3, &T, handPos, &bonus);
	
	int passed = 1;
	
	// Current player should receive exactly 2 cards
	printf("- Cards in player %i hand increased by 1 (2 drawn, 1 discarded)\n", curPlayer);
	if(!assertTest(G->handCount[curPlayer] + cardsDrawn - discarded, T.handCount[curPlayer])) {
		passed = 0;
	}

	// New cards should be treasure cards
	int initTreasure = 0;
	for(i = 0; i < G->handCount[curPlayer]; i++) {
		if(G->hand[curPlayer][i] == copper || G->hand[curPlayer][i] == silver
			|| G->hand[curPlayer][i] == gold) {
				initTreasure++;
			}
	}
	
	int newTreasure = 0;
	for(i = 0; i < T.handCount[curPlayer]; i++) {
		if(T.hand[curPlayer][i] == copper || T.hand[curPlayer][i] == silver
			|| T.hand[curPlayer][i] == gold) {
				newTreasure++;
			}
	}
	
	printf("- Player %i has two more treasure cards in hand than before\n", curPlayer);
	if(!assertTest(initTreasure + 2, newTreasure)) {
		passed = 0;
	}
	
	// Player's total deck size should not change
	printf("- Player %i total cards in all piles unchanged\n", curPlayer);
	int initDeck = G->handCount[curPlayer] + G->deckCount[curPlayer]
		+ G->discardCount[curPlayer] + G->playedCardCount;
	int newDeck = T.handCount[curPlayer] + T.deckCount[curPlayer]
		+ T.discardCount[curPlayer] + T.playedCardCount;
	if(!assertTest(initDeck, newDeck)) {
		passed = 0;
	}
		
		
	// Number of played cards should increase by 1
	printf("- Played card count increased by 1\n");
	if(!assertTest(G->playedCardCount + discarded, T.playedCardCount)) {
		passed = 0;
	}
	
	// The state of other players should not have changed
	for(i = 0; i < G->numPlayers; i++) {
		// Check state for every player other than current player
		if(i != curPlayer) {
			printf("- Player %i gamestate unchanged\n", i);
			
			if(!assertTest(G->handCount[i], T.handCount[i])) {
				passed = 0;
			}
			if(!assertTest(G->deckCount[i], T.deckCount[i])) {
				passed = 0;
			}
			if(!assertTest(G->discardCount[i], T.discardCount[i])) {
				passed = 0;
			}
		}
	}
	
	printf("- All other card supplies unchanged\n");
	int otherSupply = 1;
	for(i = 0; i < 27; i++) {
		// If supply has changed
		if(G->supplyCount[i] != T.supplyCount[i]) {
			otherSupply = 0;
		}
	}
	
	if(!assertTest(1, otherSupply)) {
		passed = 0;
	}
	
	printf("# End Subtest #\n");
	
	return passed;
}


void resetGamestate(struct gameState *G) {
	int i;
	// Set all supplies to 5
	for(i = 0; i < 27; i++) {
		G->supplyCount[i] = 5;
	}
	
	G->numPlayers = 3;
	G->playedCardCount = 0;
	G->whoseTurn = 0;
	
	// Empty all player hands
	for(i = 0; i < G->numPlayers; i++) {
		int j;
		for(j = 0; j < G->handCount[i]; j++) {
			G->hand[i][j] = -1;
		}
		for(j = 0; j < G->deckCount[i]; j++) {
			G->deck[i][j] = -1;
		}
		for(j = 0; j < G->discardCount[i]; j++) {
			G->discard[i][j] = -1;
		}
		
		G->handCount[i] = 0;
		G->deckCount[i] = 0;
		G->discardCount[i] = 0;
	}
	
	// Player 0 will be set manually
	
	int player = 1;
	// Player 1 has 5 cards in hand, 5 in deck
	for(i = 0; i < 5; i++) {
		G->hand[player][G->handCount[player]++] = village;
		G->deck[player][G->deckCount[player]++] = village;
	}
	
	player++;
	// Player 2 has 10 cards in deck
	for(i = 0; i < 10; i++) {
		G->deck[player][G->deckCount[player]++] = estate;
	}
	
}


int assertTest(int expected, int observed) {
	if (expected != observed) {
		printf(">>> Test failed -> Expected: %i\tObserved: %i\n", expected, observed);
		
		return 0;
	}
	else {
		if(NOISY_TEST) {
			printf("Test passed -> Expected: %i\tObserved: %i\n", expected, observed);
		}
		return 1;
	}
}