#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 0

#define TESTCARD "cutpurse"

int assertTest(int expected, int observed);
void resetGamestate(struct gameState *G);
int testCutpurse(struct gameState *G);

int main() {
	int i;
	struct gameState G;
	
	printf("\n[ Testing card %s ]\n", TESTCARD);
	
	int passed = 1;
	
	resetGamestate(&G);
	
	int curPlayer = 0;
	
	printf("Current player = player %i\n", curPlayer);
	
	// Both have copper
	printf("\n# Both Other Players Have Copper #\n");
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	G.hand[1][2] = copper;
	G.hand[1][4] = copper;
	G.hand[2][0] = copper;
	if(!assertTest(1, testCutpurse(&G))) {
		passed = 0;
	}
	
	// Neither have copper
	resetGamestate(&G);
	printf("\n# No Other Player Has Copper #\n");
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	if(!assertTest(1, testCutpurse(&G))) {
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


int testCutpurse(struct gameState *G) {
	struct gameState T;
	int i;
	
	int curPlayer = 0;
	int bonusCoin = 2;
	int discarded = 1;
	
	memcpy(&T, G, sizeof(struct gameState));
	
	// Testing variables
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	cardEffect(cutpurse, choice1, choice2, choice3, &T, handPos, &bonus);
	
	int passed = 1;
	
	// Current player gains +2 coin for this turn
	printf("- Player %i coin increased by 2 \n", curPlayer);
	if(!assertTest(G->coins + bonusCoin, T.coins)) {
		passed = 0;
	}
	
	// Player hand changes
	printf("- Cards in player %i hand decreased by 1 (1 discarded)\n", curPlayer);
	if(!assertTest(G->handCount[curPlayer] - discarded, T.handCount[curPlayer])) {
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
	
	// Current player's discard pile should increase by 1
	printf("- Player %i discard pile increased by 1\n", curPlayer);
	if(!assertTest(G->discardCount[curPlayer] + discarded, T.discardCount[curPlayer])) {
		passed = 0;
	}
	
	// Check state of other players
	for(i = 0; i < G->numPlayers; i++) {
		// Check state for every player other than current player
		if(i != curPlayer) {
			int j;
			int initCopper = 0;
			// Count number of copper cards in initial hand
			for(j = 0; j < G->handCount[i]; j++) {
				if(G->hand[i][j] == copper) {
					initCopper++;
				}
			}
			
			// Confirm test hand is missing a copper
			int newCopper = 0;
			for(j = 0; j < T.handCount[i]; j++) {
				if(T.hand[i][j] == copper) {
					newCopper++;
				}
			}
			printf("- Player %i gamestate changes\n", i);
			
			// Used to calculate changes if player had copper
			int change = 0;
			if(initCopper) {
				change = 1;
			}
			
			printf("Copper Count\n");
			if(!assertTest(initCopper - change, newCopper)) {
				passed = 0;
			}
			printf("Hand Count\n");
			if(!assertTest(G->handCount[i] - change, T.handCount[i])) {
				passed = 0;
			}
			printf("Discard Count\n");
			if(!assertTest(G->discardCount[i] + change, T.discardCount[i])) {
				passed = 0;
			}
			printf("Deck Count\n");
			if(!assertTest(G->deckCount[i], T.deckCount[i])) {
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