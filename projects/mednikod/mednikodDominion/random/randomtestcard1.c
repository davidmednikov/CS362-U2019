#include "../src/dominion.h"
#include "../src/dominion_helpers.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/rngs.h"

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
        int kingdomsAdded = 0;
        int result, currentPlayer, numBuys, numCoins, handCount;
        bool doesDiscard, hasEstate, exists;
        struct gameState state;

        printf ("TESTING cardEffect_Baron():\n");

        seed = (rand() % 3) - 1; // 1/3 of the time, the seed will be system TIME (-1)
        if (seed != -1) {
            seed = rand() % 1000000; // 2/3 of the time, pick a random seed  between 0 and 999,999
        }
        numPlayers = (rand() % (MAX_PLAYERS - 1)) + 2;
        for (j = 0; j < 10; j++) {
            int kingdomCard = ((rand() % 20) + 7); // number between 7 and 26 inclusive
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

        doesDiscard = rand() % 2; // randomize whether or not the player chooses to discard a card
        if (doesDiscard) {
            hasEstate = rand() % 2; // if player chooses to discard a card, randomize if they have an estate to discard or not
            if (hasEstate) {

#if (NOISY_TEST == 1)
                printf("Test player discarding an estate.\n");
#endif

                state.handCount[currentPlayer] = 2;
                state.hand[currentPlayer][0] = baron;
                state.hand[currentPlayer][1] = estate;
            } else {

#if (NOISY_TEST == 1)
                printf("Test player discarding an estate when they don't have one.\n");
#endif

                 state.handCount[currentPlayer] = 1;
                 state.hand[currentPlayer][0] = baron;
            }
        } else {

#if (NOISY_TEST == 1)
        printf("Test player not discarding an estate.\n");
#endif

                hasEstate = false; // does not discard and no estate, so gain an estate
                state.handCount[currentPlayer] = 1;
                state.hand[currentPlayer][0] = baron;
        }

        numBuys = state.numBuys;
        numCoins = state.coins;
        handCount = state.handCount[currentPlayer];

        result = cardEffect_Baron(currentPlayer, doesDiscard, &state);

#if (NOISY_TEST == 1)
        printf("expected numBuys = %d, actual numBuys = %d\n", numBuys + 1, state.numBuys);
#endif
        assertTrue(numBuys + 1 == state.numBuys); // check if the return value is correct


        if (hasEstate) {

#if (NOISY_TEST == 1)
                printf("expected handCount = %d, actual handCount = %d\n", handCount - 2, state.handCount[currentPlayer]);
#endif
                assertTrue(state.handCount[currentPlayer] == handCount - 2); // check if the return value is correct

#if (NOISY_TEST == 1)
                printf("expected coins = %d, actual coins = %d\n", numCoins + 4, state.coins);
#endif
                assertTrue(numCoins + 4 == state.coins);

        } else {

#if (NOISY_TEST == 1)
                printf("expected handCount = %d, actual handCount = %d\n", handCount, state.handCount[currentPlayer]);
#endif
                assertTrue(state.handCount[currentPlayer] == handCount);

#if (NOISY_TEST == 1)
                printf("expected last card is estate card = %d, actual last card = %d\n", estate,
                        state.hand[currentPlayer][state.handCount[currentPlayer] - 1]);
#endif
                assertTrue(estate == state.hand[currentPlayer][state.handCount[currentPlayer] - 1]);

        }


#if (NOISY_TEST == 1)
        printf("cardEffect_Baron returned = %d, expected = 0\n", result);
#endif
        assertTrue(result == 0); // check if the return value is correct

    }

    if (failedTests == 0) {
        printf("All %d tests passed!\n\n\n", tests);
    } else {
        printf("%d tests out of %d did not pass. That's a %d%% pass rate.\n\n\n", failedTests, tests, (int) (100 - failedTests * 100 / (float) tests));
    }

    return 0;

}