#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 0

#define TESTCARD "ambassador"

int assertTest(int expected, int observed);
void resetGamestate(struct gameState *G);
int testAmbassador(struct gameState *G, int choice1, int choice2);

int main() {
	int i;
	struct gameState G;
	
	resetGamestate(&G);
	
	printf("\n{ Testing card %s ]\n", TESTCARD);
	
	int passed = 1;
	
	// Testing variables
	int choice1; // the index of the card to reveal
	int choice2; // the number of copies to discard
	
	int curPlayer = 0;
	
	printf("Current player = player %i\n", curPlayer);

	printf("\n# Attempt to discard more than 2 cards #\n");
	if(!assertTest(-1, cardEffect(ambassador, village, 3, 0, &G, 0, 0))) {
		passed = 0;
	}
	
	printf("\n# Attempt to discard negative cards #\n");
	if(!assertTest(-1, cardEffect(ambassador, village, -1, 0, &G, 0, 0))) {
		passed = 0;
	}
	
	printf("\n# choice1 = handPos #\n");
	if(!assertTest(-1, cardEffect(ambassador, 0, 0, 0, &G, 0, 0))) {
		passed = 0;
	}
	
	printf("\n# Attempt to discard more cards than player has #\n");
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	G.hand[curPlayer][2] = copper;
	if(!assertTest(-1, cardEffect(ambassador, copper, 2, 0, &G, 0, 0))) {
		passed = 0;
	}
	
	// Discard 0 cards
	printf("\n# Discard no copies of card #\n");
	resetGamestate(&G);
	choice1 = 1;
	choice2 = 0;
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	if(!assertTest(1, testAmbassador(&G, choice1, choice2))) {
		passed = 0;
	}
	
	// Discard 1 card
	printf("\n# Discard 1 copy of card #\n");
	resetGamestate(&G);
	choice1 = 1;
	choice2 = 1;
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	if(!assertTest(1, testAmbassador(&G, choice1, choice2))) {
		passed = 0;
	}
	
	// Discard 1 card
	printf("\n# Discard 2 copies of card #\n");
	resetGamestate(&G);
	choice1 = 1;
	choice2 = 2;
	for(i = 0; i < 5; i++) {
		G.hand[curPlayer][G.handCount[curPlayer]++] = village;
		G.deck[curPlayer][G.deckCount[curPlayer]++] = village;
	}
	if(!assertTest(1, testAmbassador(&G, choice1, choice2))) {
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

int testAmbassador(struct gameState *G, int choice1, int choice2) {
	struct gameState T;
	int i;
	
	int curPlayer = 0;
	int discarded = 1;
	
	int revealedCard = G->hand[curPlayer][choice1];
	
	memcpy(&T, G, sizeof(struct gameState));
	
	// Testing variables
	int handPos = 0, choice3 = 0, bonus = 0;
	cardEffect(ambassador, choice1, choice2, choice3, &T, handPos, &bonus);
	
	int passed = 1;
	
	// Player hand changes
	printf("- Cards in player %i hand decreased by %i\n", curPlayer, choice2 + 1);
	if(!assertTest(G->handCount[curPlayer] - (choice2 + 1), T.handCount[curPlayer])) {
		passed = 0;
	}
	
	// Player deck doesn't change
	printf("- Cards in player %i deck unchanged\n", curPlayer);
	if(!assertTest(G->deckCount[curPlayer], T.deckCount[curPlayer])) {
		passed = 0;
	}
	
	// Player's total cards should change
	printf("- Player %i total cards in all piles decreases by %i\n", curPlayer, choice2);
	int initDeck = G->handCount[curPlayer] + G->deckCount[curPlayer]
		+ G->discardCount[curPlayer] + G->playedCardCount;
	int newDeck = T.handCount[curPlayer] + T.deckCount[curPlayer]
		+ T.discardCount[curPlayer] + T.playedCardCount;
	if(!assertTest(initDeck - choice2, newDeck)) {
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
	
	// Other players should have gained a copy of the card
	for(i = 0; i < G->numPlayers; i++) {
		// Check state for every player other than current player
		if(i != curPlayer) {
			int j;
			int initCopies = 0;
			for(j = 0; j < G->handCount[i]; j++) {
				if(G->hand[i][j] == revealedCard) {
					initCopies++;
				}
			}
			
			int newCopies = 0;
			for(j = 0; j < T.handCount[i]; j++) {
				if(T.hand[i][j] == revealedCard) {
					newCopies++;
				}
			}
			
			printf("- Player %i gains a copy of revealed card\n", i);
			if(!assertTest(initCopies + 1, newCopies)) {
					passed = 0;
			}
			printf("Hand Count\n");
			if(!assertTest(G->handCount[i] + 1, T.handCount[i])) {
				passed = 0;
			}
			printf("Discard Count\n");
			if(!assertTest(G->discardCount[i], T.discardCount[i])) {
				passed = 0;
			}
			printf("Deck Count\n");
			if(!assertTest(G->deckCount[i], T.deckCount[i])) {
					passed = 0;
			}
		}
	}
	
	// Supply count for revealed card changes
	printf("- Supply Count for Revealed Card Increases by %i\n", choice2);
	if(!assertTest(G->supplyCount[revealedCard] + choice2, T.supplyCount[revealedCard])) {
		passed = 0;
	}
	
	printf("- Only one supply pile changes\n");
	int changedPiles = 0;
	for(i = 0; i < 27; i++) {
		// If supply has changed
		if(G->supplyCount[i] != T.supplyCount[i]) {
			changedPiles++;
		}
	}
	
	if(!assertTest(1, changedPiles)) {
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