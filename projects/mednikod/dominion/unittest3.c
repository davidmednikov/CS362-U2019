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
    int result, currentPlayer, numBuys, numCoins, handCount;
    bool doesDiscard;
    int kingdoms[10] = {adventurer, council_room, feast, gardens, mine
       , remodel, smithy, village, baron, great_hall};
    struct gameState state;

    printf ("TESTING baronAction():\n");
    currentPlayer = 0;

#if (NOISY_TEST == 1)
    printf("Test player discarding an estate.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    doesDiscard = true;
    numBuys = state.numBuys;
    numCoins = state.coins;
    handCount = state.handCount[currentPlayer];
    state.handCount[currentPlayer] = 2;
    state.hand[currentPlayer][0] = baron;
    state.hand[currentPlayer][1] = estate;
    result = baronAction(currentPlayer, doesDiscard, &state);

#if (NOISY_TEST == 1)
    printf("expected numBuys = %d, actual numBuys = %d\n", numBuys + 1, state.numBuys);
#endif
    assertTrue(numBuys + 1 == state.numBuys); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected handCount = 0, actual handCount = %d\n", state.handCount[currentPlayer]);
#endif
    assertTrue(state.handCount[currentPlayer] == 0); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected coins = %d, actual coins = %d\n", numCoins + 4, state.coins);
#endif
    assertTrue(numCoins + 4 == state.coins); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("baronAction returned = %d, expected = 0\n", result);
#endif
    assertTrue(result == 0); // check if the return value is correct


#if (NOISY_TEST == 1)
    printf("Test player discarding an estate when they don't have one.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    doesDiscard = true;
    numBuys = state.numBuys;
    state.handCount[currentPlayer] = 1;
    state.hand[currentPlayer][0] = baron;
    result = baronAction(currentPlayer, doesDiscard, &state);

#if (NOISY_TEST == 1)
    printf("expected numBuys = %d, actual numBuys = %d\n", numBuys + 1, state.numBuys);
#endif
    assertTrue(numBuys + 1 == state.numBuys); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected handCount = 2, actual handCount = %d\n", state.handCount[currentPlayer]);
#endif
    assertTrue(state.handCount[currentPlayer] == 2); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected last card is estate card = %d, actual last card = %d\n", estate, state.hand[currentPlayer][1]);
#endif
    assertTrue(estate == state.hand[currentPlayer][1]); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("baronAction returned = %d, expected = 0\n", result);
#endif
    assertTrue(result == 0); // check if the return value is correct


#if (NOISY_TEST == 1)
    printf("Test player not discarding an estate.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    doesDiscard = false;
    numBuys = state.numBuys;
    handCount = state.handCount[currentPlayer];
    result = baronAction(currentPlayer, doesDiscard, &state);

#if (NOISY_TEST == 1)
    printf("expected numBuys = %d, actual numBuys = %d\n", numBuys + 1, state.numBuys);
#endif
    assertTrue(numBuys + 1 == state.numBuys); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected handCount = %d, actual handCount = %d\n", handCount + 1, state.handCount[currentPlayer]);
#endif
    assertTrue(handCount == state.handCount[currentPlayer]); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected last card is estate card = %d, actual last card = %d\n", estate, state.hand[currentPlayer][handCount]);
#endif
    assertTrue(estate == state.hand[currentPlayer][handCount]); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("baronAction returned = %d, expected = 0\n", result);
#endif
    assertTrue(result == 0); // check if the return value is correct


    if (failedTests == 0) {
        printf("All %d tests passed!\n\n\n", tests);
    } else {
        printf("%d tests out of %d did not pass. That's a %d%% pass rate.\n\n\n", failedTests, tests, (int) (100 - failedTests * 100 / (float) tests));
    }

    return 0;
}