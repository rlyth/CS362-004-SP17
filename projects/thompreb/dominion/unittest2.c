#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 0

void resetPlayerState(struct gameState *G, int player);
int assertTest(int expected, int observed);

int main () {
	struct gameState G;
	int player = 0;

printf ("\n[ Testing function fullDeckCount ]\n");
	
	int passed = 1;
	
	resetPlayerState(&G, player);
	
	// Player with no cards
	printf("\n# Testing Player With No Cards #\n");
	if(!assertTest(0, fullDeckCount(player, estate, &G))) {
		passed = 0;
	}
	
	// Player with cards but not the target card
	printf("\n# Testing Cards in all Locations but not Target Card #\n");
	G.hand[player][G.handCount[player]++] = copper;
	G.deck[player][G.deckCount[player]++] = copper;
	G.discard[player][G.discardCount[player]++] = copper;
	if(!assertTest(0, fullDeckCount(player, estate, &G))) {
		passed = 0;
	}
	
	// Target card in hand
	resetPlayerState(&G, player);
	printf("\n# Testing Target Card in Hand #\n");
	G.hand[player][G.handCount[player]++] = copper;
	if(!assertTest(1, fullDeckCount(player, copper, &G))) {
		passed = 0;
	}
	
	// Target card in deck
	resetPlayerState(&G, player);
	printf("\n# Testing Target Card in Deck #\n");
	G.deck[player][G.deckCount[player]++] = copper;
	if(!assertTest(1, fullDeckCount(player, copper, &G))) {
		passed = 0;
	}
	
	// Target card in discard
	resetPlayerState(&G, player);
	printf("\n# Testing Target Card in Discard #\n");
	G.discard[player][G.discardCount[player]++] = copper;
	if(!assertTest(1, fullDeckCount(player, copper, &G))) {
		passed = 0;
	}
	
	// Target card in all locations
	resetPlayerState(&G, player);
	printf("\n# Target Card in all Locations #\n");
	G.hand[player][G.handCount[player]++] = copper;
	G.deck[player][G.deckCount[player]++] = copper;
	G.discard[player][G.discardCount[player]++] = copper;
	if(!assertTest(3, fullDeckCount(player, copper, &G))) {
		passed = 0;
	}
	
	// Testing with more cards
	resetPlayerState(&G, player);
	printf("\n# Realistic Hand Size #\n");
	int i;
	for(i = 0; i < 5; i++) {
		if(i == 3) {
			G.hand[player][G.handCount[player]++] = smithy;
		}
		else {
			G.hand[player][G.handCount[player]++] = copper;
		}
		if(i == 4) {
			G.deck[player][G.deckCount[player]++] = smithy;
		}
		else {
			G.deck[player][G.deckCount[player]++] = duchy;
		}
		if(i == 1) {
			G.discard[player][G.discardCount[player]++] = village;
		}
		else {
			G.discard[player][G.discardCount[player]++] = smithy;
		}
	}
	if(!assertTest(6, fullDeckCount(player, smithy, &G))) {
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