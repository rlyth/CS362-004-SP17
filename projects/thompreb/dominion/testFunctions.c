#include "testFunctions.h"


void randomState(struct gameState *G, int hand, int deck, int discard) {
	// If a negative value is given, use maximums as default
	if(hand < 0) {
		hand = MAX_HAND;
	}
	if(deck < 0) {
		deck = MAX_DECK;
	}
	if(discard < 0) {
		discard = MAX_DECK;
	}
	
	memset(G, 0, sizeof(struct gameState));
	
	// numPlayers random value in range (1, 4)
	G->numPlayers = (rand() % 4) + 1;
	
	G->whoseTurn = rand() % G->numPlayers;
	
	int i;
	for(i = 0; i < treasure_map+1; i++) {
		// supplyCount random value in range (-1, 20)
		G->supplyCount[i] = (rand() % 22) - 1; 
	}
	
	for(i = 0; i < treasure_map+1; i++) {
		// supplyCount random value in range (-1, 20)
		G->embargoTokens[i] = (rand() % 22) - 1; 
	}
	
	G->outpostPlayed = rand() % 20;
	G->outpostTurn = rand() % 5;
	
	G->phase = rand() % 5;
	G->numActions = rand() % 10;
	G->numBuys = rand() % 10;
	
	int k[27] = {curse, estate, duchy, province, copper, silver, gold, adventurer, council_room,
		feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward, 
		tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map};
	
	// Populate player piles
	for(i = 0; i < G->numPlayers; i++) {
		G->handCount[i] = 0;
		G->deckCount[i] = 0;
		G->discardCount[i] = 0;
		
		int j;
		// Fill hand with random number of random cards
		for (j = 0; j < Random() * hand; j++) {
			G->hand[i][G->handCount[i]++] = k[rand() % 27];
		}
		
		// Fill deck with random number of random cards
		for (j = 0; j < Random() * deck; j++) {
			G->deck[i][G->deckCount[i]++] = k[rand() % 27];
		}
		
		// Fill discard with random number of random cards
		for (j = 0; j < Random() * discard; j++) {
			G->discard[i][G->discardCount[i]++] = k[rand() % 27];
		}
	}
	
	G->playedCardCount = 0;
	for (i = 0; i < rand() % 20; i++) {
		G->playedCards[G->playedCardCount++] = k[rand() % 27];
	}

	// Coins should accurately reflect current player's hand
	updateCoins(G->whoseTurn, G, 0);
}


void randomCards(struct gameState *G, int player, int hand, int deck, int discard) {
	int i;
	int k[27] = {curse, estate, duchy, province, copper, silver, gold, adventurer, council_room,
		feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward, 
		tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map};
	
	
	// Fill hand with specified number of random cards
	for (i = 0; i < hand; i++) {
		//printf("!! hand %i\n", i);
		G->hand[player][G->handCount[player]++] = k[rand() % 27];
	}
	
	// Fill deck with specified number of random cards
	for (i = 0; i < deck; i++) {
		//printf("!! deck %i\n", i);
		G->deck[player][G->deckCount[player]++] = k[rand() % 27];
	}
	
	// Fill discard with specified number of random cards
	for (i = 0; i < discard; i++) {
		//printf("!! discard %i\n", k[rand() % 27]);
		G->discard[player][G->discardCount[player]++] = k[rand() % 27];
	}
}


int compareGameState(struct gameState obs, struct gameState exp) {
	int passed = 1;
	
	if(obs.numPlayers != exp.numPlayers) {
		printf("FAILED: numPlayers does not match\n");
		
		passed = 0;
	}
	
	if(obs.whoseTurn != exp.whoseTurn) {
		printf("FAILED: whoseTurn does not match\n");
		
		passed = 0;
	}
	
	if(obs.phase != exp.phase) {
		printf("FAILED: phase does not match\n");
		
		passed = 0;
	}
	
	if(obs.numActions != exp.numActions) {
		printf("FAILED: numActions does not match\n");
		
		passed = 0;
	}
	
	if(obs.coins != exp.coins) {
		printf("FAILED: coins does not match\n");
		
		passed = 0;
	}
	
	if(obs.numBuys != exp.numBuys) {
		printf("FAILED: numBuys does not match\n");
		
		passed = 0;
	}
	
	if(obs.playedCardCount != exp.playedCardCount) {
		printf("FAILED: playedCardCount does not match\n");
		assertTest(exp.playedCardCount, obs.playedCardCount);
		
		passed = 0;
	}
	
	int i;
	for(i = 0; i < obs.numPlayers; i++) {
		if(obs.handCount[i] != exp.handCount[i]) {
			printf("FAILED: player %i handCount does not match\n", i);
			assertTest(exp.handCount[i], obs.handCount[i]);
			
			passed = 0;
		}
		
		/*
		if(obs.deckCount[i] != exp.deckCount[i]) {
			printf("FAILED: player %i deckCount does not match\n", i);
			
			passed = 0;
		}
		
		if(obs.discardCount[i] != exp.discardCount[i]) {
			printf("FAILED: player %i discardCount does not match\n", i);
			
			passed = 0;
		}*/
		
		// Since it's hard to predict whether discard will be shuffled back into deck,
		// I'm testing for the sum of the two piles instead
		if(obs.discardCount[i] + obs.deckCount[i] != exp.discardCount[i] + exp.deckCount[i]) {
			printf("FAILED: player %i discardCount + deckCount does not match\n", i);
			assertTest(exp.discardCount[i] + exp.deckCount[i], obs.discardCount[i] + obs.deckCount[i]);
			
			passed = 0;
		}
	}
	
	for(i = 0; i < treasure_map+1; i++) {
		if(obs.supplyCount[i] != exp.supplyCount[i]) {
			printf("FAILED: supplyCount %i does not match\n", i);
			
			passed = 0;
		}
	}
	
	return passed;
}


int cardsIn(int pile[], int pileSize, int card) {
	int count = 0, i;
	
	
	for(i = 0; i < pileSize; i++) {
		if(pile[i] == card) {
			count++;
		}
	}
	
	return count;
}


int assertTest(int expected, int observed) {
	if (expected != observed) {
		//printf(">>> Expected: %i\tObserved: %i\n", expected, observed);
			
		return 0;
	}
	else {
		//printf("Test passed -> Expected: %i\tObserved: %i\n", expected, observed);
		return 1;
	}
}