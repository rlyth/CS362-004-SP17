#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testFunctions.h"

#define TESTCARD "ambassador"

void expectedChange(struct gameState*, int, int);

int main () {
	struct gameState G, T;

	printf("\n[ Testing card %s ]\n", TESTCARD);
	
	int failed = 0;
	
	int i;
	for(i = 0; i < 1000; i++) {
		printf("# Random Test #%i\n", i+1);
		
		// Randomize gameState
		// Hand is smaller to give higher chance of insufficient cards
		randomState(&G, rand() % 50, -1, -1);
		
		printf("State -- Players: %i Active Player: %i\n", G.numPlayers, G.whoseTurn);
		
		// Make a copy of new gameState
		memcpy(&T, &G, sizeof(struct gameState));
		
		// Position of card in hand
		int trashedPos = Random() * G.handCount[G.whoseTurn];
		// The type of card being trashed
		int trashedCard = G.hand[G.whoseTurn][trashedPos];
		// Number of cards to trash (-1 : 3)
		int trashedNum = (rand() % 5) - 1;
		
		// Perform card action
		int res = playAmbassador(&G, trashedPos, trashedNum, 0);
		
		// Expect playAmbassador to fail under certain conditions
		// 1. attempted to trash card being played
		// 2. attempted to trash more than 2 or less than 0 cards
		// 3. attempted to trash more cards than player has in hand
		if(trashedPos == 0 || trashedNum > 2 ||  trashedNum < 0 ||
			cardsIn(G.hand[G.whoseTurn], G.handCount[G.whoseTurn], trashedCard) < trashedNum) {
			if(res >= 0) {
				printf("FAILED: playAmbassador did not return -1\n");
				failed++;
			}
			else {
				//printf("SUCCESS: playAmbassador returned -1\n");
			}
		}
		else {
			expectedChange(&T, trashedCard, trashedNum);
		
			if(compareGameState(G, T)) {
				printf("- All subtests passed\n");
			}
			else {
				printf("- Some subtests failed\n");
				failed++;
			}
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


void expectedChange(struct gameState *T, int trashedCard, int trashedNum) {
	int curPlayer = T->whoseTurn;
	
	// Lose 1 - 3 cards
	T->handCount[curPlayer] -= trashedNum + 1;
	
	// Cards are returned to supply
	T->deckCount[curPlayer] -= trashedNum;
	
	// Supply gains trashedNum cards
	T->supplyCount[trashedCard] += trashedNum;
	
	T->playedCardCount++;
	
	int i;
	for(i = 0; i < T->numPlayers; i++) {
		// All other players gain a copy of the card
		if(i != curPlayer) {
			// If supply has not run out
			if(T->supplyCount[trashedCard] > 0) {
				T->handCount[i]++;
				T->supplyCount[trashedCard]--;
			}
		}
	}
	
}