#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 0

void resetGameState(struct gameState *G, int player);
int checkDiscard(int handPos, int currentPlayer, int trashFlag, struct gameState *G, struct gameState *T);
int assertTest(int expected, int observed);
int testGainCard(int supplyPos, struct gameState *G, int toFlag, int player);

int main () {	
	struct gameState G;
	int player = 0;
	
	resetGameState(&G, player);

	printf ("\n[ Testing function gainCard ]\n");
	
	int passed = 1;
	
	// Check when supply pile is empty
	printf("\n# Testing Empty Supply Pile #\n");
	G.supplyCount[smithy] = 0;
	if(!assertTest(-1, gainCard(smithy, &G, 0, player))) {
		passed = 0;
	}
	
	// Check when supply pile is negative
	printf("\n# Testing Negative Supply Pile #\n");
	G.supplyCount[copper] = -1;
	if(!assertTest(-1, gainCard(copper, &G, 0, player))) {
		passed = 0;
	}
	
	// Check when added to discard
	resetGameState(&G, player);
	printf("\n# Testing Gain Card to Discard Pile #\n");
	if(!assertTest(1, testGainCard(estate, &G, 0, player))) {
		passed = 0;
	}
	
	// Check when added to deck
	resetGameState(&G, player);
	printf("\n# Testing Gain Card to Deck Pile #\n");
	if(!assertTest(1, testGainCard(estate, &G, 1, player))) {
		passed = 0;
	}
	
	// Check when added to hand
	resetGameState(&G, player);
	printf("\n# Testing Gain Card to Hand Pile #\n");
	if(!assertTest(1, testGainCard(estate, &G, 2, player))) {
		passed = 0;
	}
	
	// Check when flag is invalid
	resetGameState(&G, player);
	printf("\n# Testing Invalid toFlag #\n");
	if(!assertTest(1, testGainCard(estate, &G, 10, player))) {
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


/* Calls gainCard using parameters and checks for relevant conditions */
int testGainCard(int supplyPos, struct gameState *G, int toFlag, int player) {
	struct gameState T;
	
	int passed = 1;
	
	// Make a copy of gameState to test on
	memcpy(&T, G, sizeof(struct gameState));
	
	// Successful return
	printf("- Testing return value\n");
	if(!assertTest(0, gainCard(supplyPos, &T, toFlag, player))) {
		passed = 0;
	}
	
	// Set appropriate pile increase
	int discard = 0;
	int deck = 0;
	int hand = 0;
	switch(toFlag) {
		case 0:
			discard++;
			break;
		case 1:
			deck++;
			break;
		case 2:
			hand++;
			break;
	}
	
	// Check pile totals updated properly
	printf("- Player %i discard should change by %i\n", player, discard);
	if(!assertTest(G->discardCount[player] + discard, T.discardCount[player])) {
		passed = 0;
	}
	printf("- Player %i deck should change by %i\n", player, deck);
	if(!assertTest(G->deckCount[player] + deck, T.deckCount[player])) {
		passed = 0;
	}
	printf("- Player %i hand should change by %i\n", player, hand);
	if(!assertTest(G->handCount[player] + hand, T.handCount[player])) {
		passed = 0;
	}
	
	// check supply count updated
	printf("- Supply for card %i should decrease by 1\n", supplyPos);
	if(!assertTest(G->supplyCount[supplyPos] - 1, T.supplyCount[supplyPos])) {
		passed = 0;
	}
	
	// Check every other supply pile
	int i, otherSupply = 1;
	printf("- All other card supplies unchanged\n");
	for(i = 0; i < 27; i++) {
		// Every card that isn't target card
		if(i != supplyPos) {
			// If supply has changed
			if(G->supplyCount[i] != T.supplyCount[i]) {
				otherSupply = 0;
			}
		}
	}
	// Prints if supply fails
	if(!assertTest(otherSupply, 1)) {
		passed = 0;
	}
	
	printf("# End Subtest #\n");
	
	return passed;
}


/* Sets all supplies to 5 and empties player's hand */
void resetGameState(struct gameState *G, int player) {
	int i;
	// Set all supplies to 5
	for(i = 0; i < 27; i++) {
		G->supplyCount[i] = 5;
	}
	
	// Empty player's hand, discard, and deck
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