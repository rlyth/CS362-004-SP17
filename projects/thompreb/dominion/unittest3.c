#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 0


void resetSupplies(struct gameState* G);
int assertTest(int expected, int observed);

int main () {	
	struct gameState G;
	resetSupplies(&G);

	printf ("\n[ Testing function isGameOver ]\n");
	
	int passed = 1;
	
	// No piles are empty
	printf("\n# Testing with no empty piles #\n");
	if(!assertTest(0, isGameOver(&G))) {
		passed = 0;
	}
	
	// Province pile is empty
	printf("\n# Testing with empty province pile #\n");
	G.supplyCount[province] = 0;
	if(!assertTest(1, isGameOver(&G))) {
		passed = 0;
	}
	
	// Province pile is negative
	printf("\n# Testing with negative province pile #\n");
	G.supplyCount[province] = -1;
	if(!assertTest(1, isGameOver(&G))) {
		passed = 0;
	}
	
	// Non-province victory pile empty
	resetSupplies(&G);
	printf("\n# Testing with empty estate pile #\n");
	G.supplyCount[estate] = 0;
	if(!assertTest(0, isGameOver(&G))) {
		passed = 0;
	}
	
	resetSupplies(&G);
	printf("\n# Testing with empty duchy pile #\n");
	G.supplyCount[duchy] = 0;
	if(!assertTest(0, isGameOver(&G))) {
		passed = 0;
	}
	
	// One kingdom pile is empty
	printf("\n# Testing with one empty kingdom pile #\n");
	resetSupplies(&G);
	G.supplyCount[cutpurse] = 0;
	if(!assertTest(0, isGameOver(&G))) {
		passed = 0;
	}
	
	// Two kingdom piles are empty
	printf("\n# Testing with two empty kingdom piles #\n");
	G.supplyCount[smithy] = 0;
	if(!assertTest(0, isGameOver(&G))) {
		passed = 0;
	}

	// Three kingdom piles are empty
	printf("\n# Testing with three empty kingdom piles #\n");
	G.supplyCount[mine] = 0;
	if(!assertTest(1, isGameOver(&G))) {
		passed = 0;
	}
	
	// More than three kingdom piles are empty
	printf("\n# Testing with more than three empty kingdom piles #\n");
	G.supplyCount[tribute] = 0;
	if(!assertTest(1, isGameOver(&G))) {
		passed = 0;
	}
	
	// Two kingdom piles are empty and one is negative
	printf("\n# Testing with two empty kingdom piles and one negative kingdom pile #\n");
	resetSupplies(&G);
	G.supplyCount[tribute] = 0;
	G.supplyCount[mine] = 0;
	G.supplyCount[smithy] = -1;
	if(!assertTest(1, isGameOver(&G))) {
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


void resetSupplies(struct gameState* G) {
	int i;
	
	// Set all supplies to 10
	// isGameOver doesn't check for valid game states so won't cause issues
	for(i = 0; i < 27; i++) {
		G->supplyCount[i] = 10;
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