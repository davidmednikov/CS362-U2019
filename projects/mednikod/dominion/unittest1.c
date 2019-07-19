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
    int result, expected, trashCard, gainCard, currentPlayer, discardCount, handCount;
    int kingdoms[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState state;

    printf ("TESTING mineAction():\n");
    currentPlayer = 0;
    for (trashCard = province; trashCard <= adventurer; trashCard++)
    {
        for (gainCard = province; gainCard <= adventurer; gainCard++)
        {
#if (NOISY_TEST == 1)
            printf("Test player trashing a %d treasure card and gaining a %d treasure card.\n", trashCard, gainCard);
#endif
            memset(&state, 23, sizeof(struct gameState));   // clear the game state
            result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
            handCount = state.handCount[currentPlayer];
            discardCount = state.discardCount[currentPlayer];
            result = mineAction(currentPlayer, trashCard, gainCard, &state, 0);

            if (trashCard < 4 || trashCard > 6 || gainCard < 4 || gainCard > 6) {
                expected = -1;
            } else if (getCost(trashCard) + 3 < getCost(gainCard)) {
                expected = -2;
            } else {
                expected = 0;
            }

#if (NOISY_TEST == 1)
            printf("mineAction returned = %d, expected = %d\n", result, expected);
#endif
            assertTrue(result == expected); // check if the return value is correct

            if (expected == 0) {
#if (NOISY_TEST == 1)
                printf("player 0 handCount = %d, expected handCount = %d\n", handCount - 1, state.handCount[currentPlayer]);
#endif
                assertTrue(handCount - 1 == state.handCount[currentPlayer]); // check if the return value is correct

#if (NOISY_TEST == 1)
                printf("player 0 discardCount = %d, expected discardCount = %d\n", discardCount + 1, state.discardCount[currentPlayer]);
#endif
                assertTrue(discardCount + 1 == state.discardCount[currentPlayer]); // check if the return value is correct
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