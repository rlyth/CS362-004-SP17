#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 0

#define TESTCARD "smithy"

int assertTest(int expected, int observed);
void resetGamestate(struct gameState *G);

int main() {
	struct gameState G, T;
	
	resetGamestate(&G);
	
	int passed = 1;
	
	printf("\n[ Testing card %s ]\n", TESTCARD);
	
	// Testing variables
	memcpy(&T, &G, sizeof(struct gameState));
	int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	cardEffect(smithy, choice1, choice2, choice3, &T, handPos, &bonus);
	
	int curPlayer = 0;
	int cardsDrawn = 3;
	int discarded = 1;
	
	printf("Current player = player %i\n", curPlayer);
	
	// Current player should receive exactly 3 cards
	printf("\n# Cards in player %i hand increased by 2 (3 drawn, 1 discarded) #\n", curPlayer);
	if(!assertTest(G.handCount[curPlayer] + cardsDrawn - discarded, T.handCount[curPlayer])) {
		passed = 0;
	}

	// Cards should come from current player's pile
	printf("\n# Cards in player %i hand came from that player's pile #\n", curPlayer);
	if(!assertTest(G.deckCount[curPlayer] - cardsDrawn, T.deckCount[curPlayer])) {
		passed = 0;
	}
	
	// Number of played cards should increase by 1
	printf("\n# Played card count increased by 1 #\n");
	if(!assertTest(G.playedCardCount + discarded, T.playedCardCount)) {
		passed = 0;
	}
	
	// Current player's discard pile should increase by 1
	printf("\n# Player %i discard pile increased by 1 #\n", curPlayer);
	if(!assertTest(G.discardCount[curPlayer] + discarded, T.discardCount[curPlayer])) {
		passed = 0;
	}
	
	// The state of other players should not have changed
	int i;
	for(i = 0; i < G.numPlayers; i++) {
		// Check state for every player other than current player
		if(i != curPlayer) {
			printf("\n# Player %i gamestate unchanged #\n", i);
			
			if(!assertTest(G.handCount[i], T.handCount[i])) {
				passed = 0;
			}
			if(!assertTest(G.deckCount[i], T.deckCount[i])) {
				passed = 0;
			}
			if(!assertTest(G.discardCount[i], T.discardCount[i])) {
				passed = 0;
			}
		}
	}

	// Victory cards and treasure cards should not change
	printf("\n# All other card supplies unchanged #\n");
	int otherSupply = 1;
	for(i = 0; i < 27; i++) {
		// If supply has changed
		if(G.supplyCount[i] != T.supplyCount[i]) {
			otherSupply = 0;
		}
	}
	
	if(!assertTest(1, otherSupply)) {
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


void resetGamestate(struct gameState *G) {
	int i;
	// Set all supplies to 5
	for(i = 0; i < 27; i++) {
		G->supplyCount[i] = 5;
	}
	
	G->numPlayers = 3;
	G->playedCardCount = 0;
	G->whoseTurn = 0;
	
	// Player 0 has 5 cards in hand, 5 in deck
	int player = 0;
	for(i = 0; i < 5; i++) {
		G->hand[player][G->handCount[player]++] = copper;
		G->deck[player][G->deckCount[player]++] = copper;
	}
	
	player++;
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