#ifndef TEST_FUNC
#define TEST_FUNC

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rngs.h"


int compareGameState(struct gameState observed, struct gameState expected);

// Hand, deck, and discard can be set to -1 to use default (maximum) values
void randomState(struct gameState *G, int hand, int deck, int discard);

int cardsIn(int pile[], int pileSize, int card);

int assertTest(int expected, int observed);

#endif