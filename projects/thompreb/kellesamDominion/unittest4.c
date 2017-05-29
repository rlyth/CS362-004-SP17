#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 0

void resetGameState(struct gameState *G);
void resetPlayerState(struct gameState *G, int player);
int assertTest(int expected, int observed);

int main () {
	struct gameState G;
	int player = 0;

	printf ("\n[ Testing function scoreFor ]\n");
	
	int passed = 1;
	
	resetPlayerState(&G, player);
	
	// Player with no cards should have score of 0
	printf("\n# Testing Player With No Cards #\n");
	if(!assertTest(0, scoreFor(player, &G))) {
		passed = 0;
	}
	
	int i;
	int expectedScore = 0;
	printf("\n# Testing Estate Cards in Hand #\n");
	for(i = 0; i < 3; i++) {
		G.hand[player][G.handCount[player]++] = estate;
		expectedScore += 1;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Duchy Cards in Hand #\n");
	for(i = 0; i < 3; i++) {
		G.hand[player][G.handCount[player]++] = duchy;
		expectedScore += 3;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Province Cards in Hand #\n");
	for(i = 0; i < 3; i++) {
		G.hand[player][G.handCount[player]++] = province;
		expectedScore += 6;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Estate Cards in Deck #\n");
	for(i = 0; i < 3; i++) {
		G.deck[player][G.deckCount[player]++] = estate;
		expectedScore += 1;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Duchy Cards in Deck #\n");
	for(i = 0; i < 3; i++) {
		G.deck[player][G.deckCount[player]++] = duchy;
		expectedScore += 3;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Province Cards in Deck #\n");
	for(i = 0; i < 3; i++) {
		G.deck[player][G.deckCount[player]++] = province;
		expectedScore += 6;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Estate Cards in Discard #\n");
	for(i = 0; i < 3; i++) {
		G.discard[player][G.discardCount[player]++] = estate;
		expectedScore += 1;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Duchy Cards in Discard #\n");
	for(i = 0; i < 3; i++) {
		G.discard[player][G.discardCount[player]++] = duchy;
		expectedScore += 3;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Province Cards in Discard #\n");
	for(i = 0; i < 3; i++) {
		G.discard[player][G.discardCount[player]++] = province;
		expectedScore += 6;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	resetPlayerState(&G, player);
	expectedScore = 0;
	printf("\n# Testing Cards in all Locations #\n");
	for(i = 0; i < 3; i++) {
		G.hand[player][G.handCount[player]++] = estate;
		G.hand[player][G.handCount[player]++] = duchy;
		G.hand[player][G.handCount[player]++] = province;
		
		G.deck[player][G.deckCount[player]++] = estate;
		G.deck[player][G.deckCount[player]++] = duchy;
		G.deck[player][G.deckCount[player]++] = province;
		
		G.discard[player][G.discardCount[player]++] = estate;
		G.discard[player][G.discardCount[player]++] = duchy;
		G.discard[player][G.discardCount[player]++] = province;
		
		expectedScore += 30;
		
		if(!assertTest(expectedScore, scoreFor(player, &G))) {
			passed = 0;
		}
	}
	
	// Testing curses
	resetPlayerState(&G, player);
	printf("\n# Testing Curse Cards in All Locations #\n");
	G.hand[player][G.handCount[player]++] = curse;
	G.deck[player][G.deckCount[player]++] = curse;
	G.discard[player][G.discardCount[player]++] = curse;
	if(!assertTest(-3, scoreFor(player, &G))) {
			passed = 0;
	}
		
	resetPlayerState(&G, player);
	printf("\n# Testing Proper Curse Subtraction #\n");
	G.hand[player][G.handCount[player]++] = curse;
	G.deck[player][G.deckCount[player]++] = curse;
	G.discard[player][G.discardCount[player]++] = curse;
	G.hand[player][G.handCount[player]++] = estate;
	G.deck[player][G.deckCount[player]++] = duchy;
	G.discard[player][G.discardCount[player]++] = province;
	if(!assertTest(7, scoreFor(player, &G))) {
			passed = 0;
	}
	
	// Testing gardens
	resetPlayerState(&G, player);
	printf("\n# Testing Garden Cards in All Locations #\n");
	G.hand[player][G.handCount[player]++] = gardens;
	G.deck[player][G.deckCount[player]++] = gardens;
	G.discard[player][G.discardCount[player]++] = gardens;
	// hand will have 21 cards total = 6 victory points
	for(i = 0; i < 6; i++) {
		G.hand[player][G.handCount[player]++] = copper;
		G.deck[player][G.deckCount[player]++] = copper;
		G.discard[player][G.discardCount[player]++] = copper;
	}
	if(!assertTest(6, scoreFor(player, &G))) {
			passed = 0;
	}
	
	// Testing great halls
	resetPlayerState(&G, player);
	printf("\n# Testing Great Hall Cards in All Locations #\n");
	G.hand[player][G.handCount[player]++] = great_hall;
	G.deck[player][G.deckCount[player]++] = great_hall;
	G.discard[player][G.discardCount[player]++] = great_hall;
	if(!assertTest(3, scoreFor(player, &G))) {
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


/* Sets all supplies to 5 */
void resetGameState(struct gameState *G) {
	int i;
	// Set all supplies to 5
	for(i = 0; i < 27; i++) {
		G->supplyCount[i] = 5;
	}
}


void resetPlayerState(struct gameState *G, int player) {
	// Empty player's hand, discard, and deck
	int i;
	for(i = 0; i < G->handCount[player]; i++) {
		G->hand[player][i] = -1;
	}
	
	for(i = 0; i < G->deckCount[player]; i++) {
		G->deck[player][i] = -1;
	}
	
	for(i = 0; i < G->discardCount[player]; i++) {
		G->discard[player][i] = -1;
	}
	
	G->handCount[player] = 0;
	G->deckCount[player] = 0;
	G->discardCount[player] = 0;
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