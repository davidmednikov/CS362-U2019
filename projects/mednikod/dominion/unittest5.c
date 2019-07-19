#include "dominion.h"
#include "dominion_helpers.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "rngs.h"

int failedTests = 0;
int tests = 0;

void assertTrue(bool expression) {
    if (expression) {
        printf("Test passed\n");
    } else {
        printf("Test failed\n");
        failedTests++;
    }
    tests++;
}

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int main() {
    int seed = 1000;
    int numPlayer = 2;
    int currentPlayer, nextPlayer, result, numActions, numCards, numCoins;
    int tributeRevealedCards[2];
    int kingdoms[10] = {adventurer, council_room, feast, gardens, mine
       , remodel, smithy, village, baron, great_hall};
    struct gameState state;

    printf ("TESTING tributeAction():\n");
    currentPlayer = 0;
    nextPlayer = 1;


#if (NOISY_TEST == 1)
    printf("Test next player having 0 cards to reveal.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.deckCount[nextPlayer] = 0;
    state.discardCount[nextPlayer] = 0;
    tributeRevealedCards[0] = -1;
    tributeRevealedCards[1] = -2;
    result = tributeAction(currentPlayer, nextPlayer, tributeRevealedCards, &state);

#if (NOISY_TEST == 1)
    printf("expected result: 0, actual result: %d\n", result);
#endif
    assertTrue(result == 0);


#if (NOISY_TEST == 1)
    printf("Test next player having 1 deck cards (1 action card) and 0 discard cards.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.deckCount[nextPlayer] = 1;
    state.discardCount[nextPlayer] = 0;
    tributeRevealedCards[0] = council_room;
    tributeRevealedCards[1] = -1;
    numActions = state.numActions;
    result = tributeAction(currentPlayer, nextPlayer, tributeRevealedCards, &state);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer deckCount = 0, actual nextPlayer deckCount = %d\n", state.deckCount[nextPlayer]);
#endif
    assertTrue(state.deckCount[nextPlayer] == 0);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer discardCount = 1, actual nextPlayer deckCount = %d\n", state.discardCount[nextPlayer]);
#endif
    assertTrue(state.discardCount[nextPlayer] == 1);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer discardCard = council_room %d, actual nextPlayer discardCard = %d\n", council_room, state.discard[nextPlayer][0]);
#endif
    assertTrue(state.discard[nextPlayer][0] == council_room);

#if (NOISY_TEST == 1)
    printf("expected numActions = %d, actual numActions = %d\n", numActions + 2, state.numActions);
#endif
    assertTrue(numActions + 2 == state.numActions);

#if (NOISY_TEST == 1)
    printf("expected result: 0, actual result: %d\n", result);
#endif
    assertTrue(result == 0);


#if (NOISY_TEST == 1)
    printf("Test next player having 0 deck cards and 1 discard cards (1 treasure card).\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.deckCount[nextPlayer] = 0;
    state.discardCount[nextPlayer] = 1;
    tributeRevealedCards[0] = gold;
    tributeRevealedCards[1] = -1;
    numCoins = state.coins;
    result = tributeAction(currentPlayer, nextPlayer, tributeRevealedCards, &state);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer deckCount = 0, actual nextPlayer deckCount = %d\n", state.deckCount[nextPlayer]);
#endif
    assertTrue(state.deckCount[nextPlayer] == 0);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer discardCount = 1, actual nextPlayer discardCount = %d\n", state.discardCount[nextPlayer]);
#endif
    assertTrue(state.discardCount[nextPlayer] == 1);

#if (NOISY_TEST == 1)
    printf("expected numCoins = %d, actual numCoins = %d\n", numCoins + 2, state.coins);
#endif
    assertTrue(numCoins + 2 == state.coins);

#if (NOISY_TEST == 1)
    printf("expected result: 0, actual result: %d\n", result);
#endif
    assertTrue(result == 0);


#if (NOISY_TEST == 1)
    printf("Test next player revealing 2 cards that are different (1 action and 1 victory).\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.deckCount[nextPlayer] = 2;
    state.discardCount[nextPlayer] = 0;
    tributeRevealedCards[0] = estate;
    tributeRevealedCards[1] = sea_hag;
    numActions = state.numActions;
    numCards = state.handCount[currentPlayer];
    result = tributeAction(currentPlayer, nextPlayer, tributeRevealedCards, &state);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer deckCount = 0, actual nextPlayer deckCount = %d\n", state.deckCount[nextPlayer]);
#endif
    assertTrue(state.deckCount[nextPlayer] == 0);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer discardCount = 2, actual nextPlayer discardCount = %d\n", state.discardCount[nextPlayer]);
#endif
    assertTrue(state.discardCount[nextPlayer] == 2);

#if (NOISY_TEST == 1)
    printf("expected currentPlayer handCount = %d, actual nextPlayer handCount = %d\n", numCards + 2, state.handCount[currentPlayer]);
#endif
    assertTrue(state.handCount[currentPlayer] == numCards + 2);

#if (NOISY_TEST == 1)
    printf("expected numActions = %d, actual numActions = %d\n", numActions + 2, state.numActions);
#endif
    assertTrue(numActions + 2 == state.numActions);

#if (NOISY_TEST == 1)
    printf("expected result: 0, actual result: %d\n", result);
#endif
    assertTrue(result == 0);


#if (NOISY_TEST == 1)
    printf("Test next player revealing 2 cards that are the same (2 golds).\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.deckCount[nextPlayer] = 2;
    state.discardCount[nextPlayer] = 0;
    tributeRevealedCards[0] = gold;
    tributeRevealedCards[1] = gold;
    numCoins = state.coins;
    result = tributeAction(currentPlayer, nextPlayer, tributeRevealedCards, &state);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer deckCount = 1, actual nextPlayer deckCount = %d\n", state.deckCount[nextPlayer]);
#endif
    assertTrue(state.deckCount[nextPlayer] == 1);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer discardCount = 1, actual nextPlayer discardCount = %d\n", state.discardCount[nextPlayer]);
#endif
    assertTrue(state.discardCount[nextPlayer] == 1);

#if (NOISY_TEST == 1)
    printf("expected numCoins = %d, actual numCoins = %d\n", numCoins + 2, state.coins);
#endif
    assertTrue(numCoins + 2 == state.coins);

#if (NOISY_TEST == 1)
    printf("expected result: 0, actual result: %d\n", result);
#endif
    assertTrue(result == 0);


#if (NOISY_TEST == 1)
    printf("Test next player revealing 2 action cards that are different (1 minion and 1 salvager).\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.deckCount[nextPlayer] = 2;
    state.discardCount[nextPlayer] = 0;
    tributeRevealedCards[0] = minion;
    tributeRevealedCards[1] = salvager;
    numActions = state.numActions;
    result = tributeAction(currentPlayer, nextPlayer, tributeRevealedCards, &state);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer deckCount = 0, actual nextPlayer deckCount = %d\n", state.deckCount[nextPlayer]);
#endif
    assertTrue(state.deckCount[nextPlayer] == 0);

#if (NOISY_TEST == 1)
    printf("expected nextPlayer discardCount = 2, actual nextPlayer discardCount = %d\n", state.discardCount[nextPlayer]);
#endif
    assertTrue(state.discardCount[nextPlayer] == 2);

#if (NOISY_TEST == 1)
    printf("expected numActions = %d, actual numActions = %d\n", numActions + 4, state.numActions);
#endif
    assertTrue(numActions + 4 == state.numActions);

#if (NOISY_TEST == 1)
    printf("expected result: 0, actual result: %d\n", result);
#endif
    assertTrue(result == 0);


    if (failedTests == 0) {
        printf("All %d tests passed!\n\n\n", tests);
    } else {
        printf("%d tests out of %d did not pass. That's a %d%% pass rate.\n\n\n", failedTests, tests, (int) (100 - failedTests * 100 / (float) tests));
    }

    return 0;
}