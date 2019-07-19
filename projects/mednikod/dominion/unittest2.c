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
    int i, j;
    int seed = 1000;
    int numPlayer = 4;
    int currentPlayer, result;
    bool getCoins, discardHand;
    int kingdoms[10] = {adventurer, council_room, feast, gardens, mine
       , remodel, smithy, village, baron, great_hall};
    struct gameState state;

    printf ("TESTING minionAction():\n");

    getCoins = true;
    discardHand = false;
    currentPlayer = 0;


#if (NOISY_TEST == 1)
    printf("Test player getting 2 cards.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    int actions = state.numActions;
    int coins = state.coins;
    int cards = state.handCount[currentPlayer];
    result = minionAction(currentPlayer, getCoins, discardHand, &state, 1);
#if (NOISY_TEST == 1)
    printf("expected actions: %d, actual actions: %d\n", actions + 1, state.numActions);
#endif
    assertTrue(actions + 1 == state.numActions); // check if actions was incremented

#if (NOISY_TEST == 1)
    printf("expected coins: %d, actual coins: %d\n", coins + 2, state.coins);
#endif
    assertTrue(coins + 2 == state.coins); // check if coins was incremented by 2

#if (NOISY_TEST == 1)
    printf("expected hand count: %d, actual hand count: %d\n", cards - 1, state.handCount[currentPlayer]);
#endif
    assertTrue(cards - 1 == state.handCount[currentPlayer]);

#if (NOISY_TEST == 1)
    printf("expected result: 0, actual result: %d\n", result);
#endif
    assertTrue(result == 0);


#if (NOISY_TEST == 1)
    printf("Test player choosing to discard cards.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    for (i = currentPlayer; i < numPlayer; i++) {
        state.handCount[i] = i + 3;
        for (j = currentPlayer; j < i + 3; j++) {
            state.hand[i][j] = minion;
        }
    }
    getCoins = false;
    discardHand = true;
    result = minionAction(currentPlayer, getCoins, discardHand, &state, 1);

    #if (NOISY_TEST == 1)
    printf("expected actions: %d, actual actions: %d\n", actions + 1, state.numActions);
#endif
    assertTrue(actions + 1 == state.numActions); // check if actions was incremented

    for (i = currentPlayer; i < numPlayer; i++) {
#if (NOISY_TEST == 1)
        printf("player %d expected hand count: 4, actual hand count: %d\n", i, state.handCount[i]);
#endif
        assertTrue(state.handCount[i] == 4); // check if each player has 4 cards
    }

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