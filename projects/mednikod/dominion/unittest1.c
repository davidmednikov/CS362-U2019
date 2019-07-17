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
    int result, expected, trashCard, gainCard;
    int kingdoms[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;

    printf ("TESTING mineAction():\n");
    for (trashCard = province; trashCard <= adventurer; trashCard++)
    {
        for (gainCard = province; gainCard <= adventurer; gainCard++)
        {
#if (NOISY_TEST == 1)
            printf("Test player discarding a %d treasure card and gaining a %d treasure card.\n", trashCard, gainCard);
#endif
            memset(&G, 23, sizeof(struct gameState));   // clear the game state
            result = initializeGame(numPlayer, kingdoms, seed, &G); // initialize a new game              // set the number of cards on hand
            result = mineAction(1, trashCard, gainCard, &G, 1);

            if (trashCard < 4 || trashCard > 6 || gainCard < 4 || gainCard > 6) {
                expected = -1;
            } else if (getCost(trashCard) + 3 >= getCost(gainCard)) {
                expected = -2;
            } else {
                expected = 0;
            }

#if (NOISY_TEST == 1)
            printf("mineAction returned = %d, expected = %d\n", result, expected);
#endif
            assertTrue(result == expected); // check if the return value is correct
        }
    }

    if (failedTests == 0) {
        printf("All %d tests passed!\n", tests);
    } else {
        printf("%d tests out of %d did not pass. That's a %d%% pass rate.\n", failedTests, tests, (int) (100 - failedTests * 100 / (float) tests));
    }

    return 0;
}