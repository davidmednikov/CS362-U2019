#include "dominion.h"
#include "dominion_helpers.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rngs.h"

# define MAX_PLAYERS 4

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
#define NOISY_TEST 0

int main() {
    int i, j, k;
    for (i = -2000; i < 2000; i++) {
        int seed, numPlayers;
        int kingdoms[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int shouldHaveFourCards[4] = {0, 0, 0, 0};
        int oldCount[4] = {0, 0, 0, 0};
        int kingdomsAdded = 0;
        int result, currentPlayer, numActions, numCoins, handCount;
        bool getCoins, discardHand, exists;
        struct gameState state;

        printf ("TESTING minionAction():\n");

        seed = (rand() % 3) - 1; // 1/3 of the time, the seed will be system TIME (-1)
        if (seed != -1) {
            seed = rand() % 1000000; // 2/3 of the time, pick a random seed  between 0 and 999,999
        }
        numPlayers = (rand() % (MAX_PLAYERS - 1)) + 2;
        for (j = 0; j < 10; j++) {
            int kingdomCard = ((rand() % 20) + 7); // number between 7 and 26 inclusive]
            if (kingdomsAdded == 0) {
                kingdoms[j] = kingdomCard;
                kingdomsAdded = 1;
            } else {
                do {
                    exists = false;
                    for (k = 0; k < kingdomsAdded; k++) {
                        if (kingdoms[k] == kingdomCard) {
                            exists = true;
                            kingdomCard = ((rand() % 20) + 7);
                            break;
                        }
                    }
                } while (exists == true);
                kingdoms[j] = kingdomCard;
                kingdomsAdded++;
            }
        }

        memset(&state, 23, sizeof(struct gameState));   // clear the game state
        result = initializeGame(numPlayers, kingdoms, seed, &state); // initialize a new game

        currentPlayer = 0;
        
        numActions = state.numActions;
        numCoins = state.coins;
        handCount = state.handCount[currentPlayer];

        getCoins = rand() % 2;
        if (getCoins) {
            discardHand = false;
        } else {
            discardHand = true;
        }

        if (discardHand) {
            for (j = currentPlayer; j < numPlayers; j++) {
                state.handCount[j] = rand() % 7;
                if (j == 0 && (state.handCount[j] == 0 || state.handCount[j] == 1)) {
                    state.handCount[j] = 2;
                }
                for (k = currentPlayer; k < state.handCount[j]; k++) {
                    state.hand[j][k] = minion;
                }
                if (j == 0 || state.handCount[j] >= 4) {
                    shouldHaveFourCards[j] = 1;
                }
                oldCount[j] = state.handCount[j];
            }

            state.playedCardCount = 0;
        }

        result = minionAction(currentPlayer, getCoins, discardHand, &state, 0);

        if (getCoins) {

#if (NOISY_TEST == 1)
    printf("Test player getting 2 coins.\n");
#endif

#if (NOISY_TEST == 1)
            printf("expected coins: %d, actual coins: %d\n", numCoins + 2, state.coins);
#endif
            assertTrue(numCoins + 2 == state.coins); // check if coins was incremented by 2

#if (NOISY_TEST == 1)
            printf("expected hand count: %d, actual hand count: %d\n", handCount - 1, state.handCount[currentPlayer]);
#endif

            assertTrue(handCount - 1 == state.handCount[currentPlayer]);

        } else {

#if (NOISY_TEST == 1)
        printf("Test player choosing to discard cards.\n");
#endif
            for (j = currentPlayer; j < numPlayers; j++) {
#if (NOISY_TEST == 1)
                printf("player %d expected hand count: %d, actual hand count: %d\n", j,
                    shouldHaveFourCards[j] ? 4 : oldCount[j], state.handCount[j]);
#endif
                assertTrue(state.handCount[j] == shouldHaveFourCards[j] ? 4 : oldCount[j]); // check if each player has 4 cards
            }
        }

#if (NOISY_TEST == 1)
        printf("expected actions: %d, actual actions: %d\n", numActions + 1, state.numActions);
#endif
        assertTrue(numActions + 1 == state.numActions); // check if actions was incremented

#if (NOISY_TEST == 1)
        printf("expected result: 0, actual result: %d\n", result);
#endif
        assertTrue(result == 0);

    }

    if (failedTests == 0) {
        printf("All %d tests passed!\n\n\n", tests);
    } else {
        printf("%d tests out of %d did not pass. That's a %d%% pass rate.\n\n\n", failedTests, tests, (int) (100 - failedTests * 100 / (float) tests));
    }

    return 0;
}