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
    int i, j;
    for (i = -2000; i < 2000; i++) {
        int seed, numPlayers;
        int kingdoms[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int kingdomsAdded = 0;
        int result, currentPlayer, numBuys, numCoins, handCount;
        bool doesDiscard;
        struct gameState state;

        printf ("TESTING baronAction():\n");

#if (NOISY_TEST == 1)
        printf("Test player discarding an estate.\n");
#endif
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
                int k;
                bool exists = false;
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
        currentPlayer = 0;
        memset(&state, 23, sizeof(struct gameState));   // clear the game state
        result = initializeGame(numPlayers, kingdoms, seed, &state); // initialize a new game
        doesDiscard = true;
        numBuys = state.numBuys;
        numCoins = state.coins;
        state.handCount[currentPlayer] = 2;
        handCount = state.handCount[currentPlayer];
        state.hand[currentPlayer][0] = baron;
        state.hand[currentPlayer][1] = estate;
        result = baronAction(currentPlayer, doesDiscard, &state);

#if (NOISY_TEST == 1)
        printf("expected numBuys = %d, actual numBuys = %d\n", numBuys + 1, state.numBuys);
#endif
        assertTrue(numBuys + 1 == state.numBuys); // check if the return value is correct

#if (NOISY_TEST == 1)
        printf("expected handCount = %d, actual handCount = %d\n", handCount - 2, state.handCount[currentPlayer]);
#endif
        assertTrue(state.handCount[currentPlayer] == handCount - 2); // check if the return value is correct

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
        seed = (rand() % 3) - 1; // 1/3 of the time, the seed will be system TIME (-1)
        if (seed != -1) {
            seed = rand() % 1000000; // 2/3 of the time, pick a random seed  between 0 and 999,999
        }
        numPlayers = (rand() % (MAX_PLAYERS - 1)) + 2;
        kingdomsAdded = 0;
        for (j = 0; j < 10; j++) {
            int kingdomCard = ((rand() % 20) + 7); // number between 7 and 26 inclusive]
            if (kingdomsAdded == 0) {
                kingdoms[j] = kingdomCard;
                kingdomsAdded = 1;
            } else {
                int k;
                bool exists = false;
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
        currentPlayer = 0;

        memset(&state, 23, sizeof(struct gameState));   // clear the game state
        result = initializeGame(numPlayers, kingdoms, seed, &state); // initialize a new game
        doesDiscard = true;
        numBuys = state.numBuys;
        state.handCount[currentPlayer] = 1;
        handCount = state.handCount[currentPlayer];
        state.hand[currentPlayer][0] = baron;
        result = baronAction(currentPlayer, doesDiscard, &state);


#if (NOISY_TEST == 1)
        printf("expected numBuys = %d, actual numBuys = %d\n", numBuys + 1, state.numBuys);
#endif
        assertTrue(numBuys + 1 == state.numBuys); // check if the return value is correct

#if (NOISY_TEST == 1)
        printf("expected handCount = %d, actual handCount = %d\n", handCount, state.handCount[currentPlayer]);
#endif
        assertTrue(state.handCount[currentPlayer] == handCount); // check if the return value is correct

#if (NOISY_TEST == 1)
        printf("expected last card is estate card = %d, actual last card = %d\n", estate,
            state.hand[currentPlayer][state.handCount[currentPlayer] - 1]);
#endif
        assertTrue(estate == state.hand[currentPlayer][state.handCount[currentPlayer] - 1]); // check if the return value is correct

#if (NOISY_TEST == 1)
        printf("baronAction returned = %d, expected = 0\n", result);
#endif
        assertTrue(result == 0); // check if the return value is correct


#if (NOISY_TEST == 1)
        printf("Test player not discarding an estate.\n");
#endif
        seed = (rand() % 3) - 1; // 1/3 of the time, the seed will be system TIME (-1)
        if (seed != -1) {
            seed = rand() % 1000000; // 2/3 of the time, pick a random seed  between 0 and 999,999
        }
        numPlayers = (rand() % (MAX_PLAYERS - 1)) + 2;
        kingdomsAdded = 0;
        for (j = 0; j < 10; j++) {
            int kingdomCard = ((rand() % 20) + 7); // number between 7 and 26 inclusive]
            if (kingdomsAdded == 0) {
                kingdoms[j] = kingdomCard;
                kingdomsAdded = 1;
            } else {
                int k;
                bool exists = false;
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
        currentPlayer = 0;

        memset(&state, 23, sizeof(struct gameState));   // clear the game state
        result = initializeGame(numPlayers, kingdoms, seed, &state); // initialize a new game
        doesDiscard = false;
        numBuys = state.numBuys;
        state.hand[currentPlayer][0] = baron;
        state.handCount[currentPlayer] = 1;
        handCount = state.handCount[currentPlayer];
        result = baronAction(currentPlayer, doesDiscard, &state);

#if (NOISY_TEST == 1)
        printf("expected numBuys = %d, actual numBuys = %d\n", numBuys + 1, state.numBuys);
#endif
        assertTrue(numBuys + 1 == state.numBuys); // check if the return value is correct

#if (NOISY_TEST == 1)
        printf("expected handCount = %d, actual handCount = %d\n", handCount, state.handCount[currentPlayer]);
#endif
        assertTrue(handCount == state.handCount[currentPlayer]); // check if the return value is correct

#if (NOISY_TEST == 1)
        printf("expected last card is estate card = %d, actual last card = %d\n", estate,
            state.hand[currentPlayer][state.handCount[currentPlayer] - 1]);
#endif
        assertTrue(estate == state.hand[currentPlayer][state.handCount[currentPlayer] - 1]);

#if (NOISY_TEST == 1)
        printf("baronAction returned = %d, expected = 0\n", result);
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