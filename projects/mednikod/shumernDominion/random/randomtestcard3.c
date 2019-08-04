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
        int result, currentPlayer, nextPlayer, numActions, numCoins, handCount;
        int nextPlayerDeckCount, nextPlayerDiscardCount;
        int tributeRevealedCards[2];
        bool exists;
        struct gameState state;

        printf ("TESTING cardEffect_Tribute():\n");

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
        nextPlayer = 1;

        nextPlayerDeckCount = rand() % 3;
        if (nextPlayerDeckCount <= 1) {
            nextPlayerDiscardCount = rand() % 2;
        }

        if (nextPlayerDeckCount == 0 && nextPlayerDiscardCount == 0) {
            state.deckCount[nextPlayer] = 0;
            state.discardCount[nextPlayer] = 0;
            tributeRevealedCards[0] = -1;
            tributeRevealedCards[1] = -1;
        } else if (nextPlayerDeckCount == 1 && nextPlayerDiscardCount == 0) {
            state.deckCount[nextPlayer] = 1;
            state.discardCount[nextPlayer] = 0;
            tributeRevealedCards[0] = (rand() % 26) + 1;
            tributeRevealedCards[1] = -1;
        } else if (nextPlayerDeckCount == 0 && nextPlayerDiscardCount == 1) {
            state.deckCount[nextPlayer] = 0;
            state.discardCount[nextPlayer] = 1;
            tributeRevealedCards[0] = (rand() % 26) + 1;
            tributeRevealedCards[1] = -1;
        } else {
            if (nextPlayerDeckCount == 1 && nextPlayerDiscardCount == 1) {
                state.deckCount[nextPlayer] = 1;
                state.discardCount[nextPlayer] = 1;
            } else {
                nextPlayerDeckCount = state.deckCount[nextPlayer];
                nextPlayerDiscardCount = state.discardCount[nextPlayer];
            }
            tributeRevealedCards[0] = (rand() % 26) + 1;
            tributeRevealedCards[1] = (rand() % 26) + 1;
        }

        numActions = state.numActions;
        numCoins = state.coins;
        handCount = state.handCount[currentPlayer];

        result = cardEffect_Tribute(currentPlayer, nextPlayer, &state);

        int firstTribute = tributeRevealedCards[0];
        if (firstTribute != -1) {
            if (firstTribute >= 4 && firstTribute <= 6) {
                numCoins += 2;
            } else if ((firstTribute >= 1 && firstTribute <= 3) || firstTribute == 10 || firstTribute == 16) {
                handCount += 2;
            } else {
                numActions += 2;
            }
        }

        int secondTribute = tributeRevealedCards[1];
        if (secondTribute != -1 && secondTribute != firstTribute) {
            if (secondTribute >= 4 && secondTribute <= 6) {
                numCoins += 2;
            } else if ((secondTribute >= 1 && secondTribute <= 3) || secondTribute == 10 || secondTribute == 16) {
                handCount += 2;
            } else {
                numActions += 2;
            }
        }


#if (NOISY_TEST == 1)
        printf("expected result: 0, actual result: %d\n", result);
#endif
        assertTrue(result == 0);

#if (NOISY_TEST == 1)
        printf("expected numActions = %d, actual numActions = %d\n", numActions, state.numActions);
#endif
        assertTrue(numActions == state.numActions);

#if (NOISY_TEST == 1)
        printf("expected numCoins = %d, actual numCoins = %d\n", numCoins, state.coins);
#endif
        assertTrue(numCoins == state.coins);

#if (NOISY_TEST == 1)
        printf("expected currentPlayer handCount = %d, actual nextPlayer handCount = %d\n", handCount, state.handCount[currentPlayer]);
#endif
        assertTrue(state.handCount[currentPlayer] == handCount);

        if (!(nextPlayerDeckCount == 0 && nextPlayerDiscardCount == 0)) { // more than 1 card to discard
            if ((nextPlayerDeckCount == 1 && nextPlayerDiscardCount) == 0 || (nextPlayerDeckCount == 0 && nextPlayerDiscardCount == 1)) {
                // only 1 card to discard
#if (NOISY_TEST == 1)
                printf("expected nextPlayer discardCount = 1, actual nextPlayer discardCount = %d\n", state.discardCount[nextPlayer]);
#endif
                assertTrue(state.discardCount[nextPlayer] == 1);

#if (NOISY_TEST == 1)
                printf("expected nextPlayer discardCard = %d, actual nextPlayer discardCard = %d\n", firstTribute, state.discard[nextPlayer][0]);
#endif
                assertTrue(state.discard[nextPlayer][0] == firstTribute);

#if (NOISY_TEST == 1)
                printf("expected nextPlayer deckCount = 0, actual nextPlayer deckCount = %d\n", state.deckCount[nextPlayer]);
#endif
                assertTrue(state.deckCount[nextPlayer] == 0);

            } else if (firstTribute != secondTribute) { // 2 cards to discard that are different
                if (nextPlayerDeckCount == 1 && nextPlayerDiscardCount == 1) {

#if (NOISY_TEST == 1)
                    printf("expected nextPlayer deckCount = 0, actual nextPlayer deckCount = %d\n", state.deckCount[nextPlayer]);
#endif
                    assertTrue(state.deckCount[nextPlayer] == 0);

#if (NOISY_TEST == 1)
                    printf("expected nextPlayer discardCount = 2, actual nextPlayer discardCount = %d\n", state.discardCount[nextPlayer]);
#endif
                    assertTrue(state.discardCount[nextPlayer] == 2);

#if (NOISY_TEST == 1)
                    printf("expected nextPlayer 1st discardCard = %d, actual nextPlayer discardCard = %d\n", firstTribute, state.discard[nextPlayer][0]);
#endif
                    assertTrue(state.discard[nextPlayer][0] == firstTribute);

#if (NOISY_TEST == 1)
                    printf("expected nextPlayer 2nd discardCard = %d, actual nextPlayer discardCard = %d\n", secondTribute, state.discard[nextPlayer][0]);
#endif
                    assertTrue(state.discard[nextPlayer][1] == secondTribute);

                } else {

#if (NOISY_TEST == 1)
                    printf("Testing next player with 2 or more cards in their deck.\n");
                    printf("expected nextPlayer deckCount = %d, actual nextPlayer deckCount = %d\n", nextPlayerDeckCount - 2, state.deckCount[nextPlayer]);
#endif
                    assertTrue(state.deckCount[nextPlayer] == nextPlayerDeckCount - 2);

#if (NOISY_TEST == 1)
                    printf("expected nextPlayer discardCount = %d, actual nextPlayer discardCount = %d\n", nextPlayerDiscardCount + 2, state.discardCount[nextPlayer]);
#endif
                    assertTrue(state.discardCount[nextPlayer] == nextPlayerDiscardCount + 2);

#if (NOISY_TEST == 1)
                    printf("expected nextPlayer 1st discardCard = %d, actual nextPlayer discardCard = %d\n", firstTribute, state.discard[nextPlayer][nextPlayerDiscardCount]);
#endif
                    assertTrue(state.discard[nextPlayer][nextPlayerDiscardCount] == firstTribute);

#if (NOISY_TEST == 1)
                    printf("expected nextPlayer 2nd discardCard = %d, actual nextPlayer discardCard = %d\n", secondTribute, state.discard[nextPlayer][nextPlayerDiscardCount + 1]);
#endif
                    assertTrue(state.discard[nextPlayer][nextPlayerDiscardCount + 1] == secondTribute);

                }

            } else { // 2 cards to discard that are the same

#if (NOISY_TEST == 1)
                printf("expected nextPlayer deckCount = %d, actual nextPlayer deckCount = %d\n", nextPlayerDeckCount - 1, state.deckCount[nextPlayer]);
#endif
                assertTrue(state.deckCount[nextPlayer] == nextPlayerDeckCount - 1);

#if (NOISY_TEST == 1)
                printf("expected nextPlayer discardCount = %d, actual nextPlayer discardCount = %d\n", nextPlayerDiscardCount + 1, state.discardCount[nextPlayer]);
#endif
                assertTrue(state.discardCount[nextPlayer] == nextPlayerDiscardCount + 1);

 #if (NOISY_TEST == 1)
                printf("expected nextPlayer 1st discardCard = %d, actual nextPlayer discardCard = %d\n", firstTribute, state.discard[nextPlayer][0]);
#endif
                assertTrue(state.discard[nextPlayer][0] == firstTribute);

            }
        }
    }

    if (failedTests == 0) {
        printf("All %d tests passed!\n\n\n", tests);
    } else {
        printf("%d tests out of %d did not pass. That's a %d%% pass rate.\n\n\n", failedTests, tests, (int) (100 - failedTests * 100 / (float) tests));
    }

    return 0;
}