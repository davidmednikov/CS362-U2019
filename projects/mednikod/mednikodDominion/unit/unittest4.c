#include "../src/dominion.h"
#include "../src/dominion_helpers.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../src/rngs.h"

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
    int i;
    int seed = 1000;
    int numPlayer = 3;
    int result, currentPlayer, cardToDiscard, copiesToDiscard, supplyCount;
    int kingdoms[10] = {adventurer, council_room, feast, gardens, mine
       , remodel, smithy, village, baron, great_hall};
    struct gameState state;

    printf ("TESTING cardEffect_Ambassador():\n");
    currentPlayer = 0;

#if (NOISY_TEST == 1)
    printf("Test player discarding ambassador card.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.handCount[currentPlayer] = 1;
    state.hand[currentPlayer][0] = ambassador;
    cardToDiscard = 0;
    copiesToDiscard = 0;
    result = cardEffect_Ambassador(currentPlayer, cardToDiscard, copiesToDiscard, &state, 0);
#if (NOISY_TEST == 1)
    printf("cardEffect_Ambassador returned = %d, expected = -1\n", result);
#endif
    assertTrue(result == -1); // check if the return value is correct


#if (NOISY_TEST == 1)
    printf("Test player discarding a negative number of cards = -1.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.handCount[currentPlayer] = 2;
    state.hand[currentPlayer][0] = ambassador;
    state.hand[currentPlayer][1] = tribute;
    cardToDiscard = 1;
    copiesToDiscard = -1;
    result = cardEffect_Ambassador(currentPlayer, cardToDiscard, copiesToDiscard, &state, 0);
#if (NOISY_TEST == 1)
    printf("cardEffect_Ambassador returned = %d, expected = -1\n", result);
#endif
    assertTrue(result == -1); // check if the return value is correct


#if (NOISY_TEST == 1)
    printf("Test player discarding too many cards = 3.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.handCount[currentPlayer] = 2;
    state.hand[currentPlayer][0] = ambassador;
    state.hand[currentPlayer][1] = tribute;
    cardToDiscard = 1;
    copiesToDiscard = 3;
    result = cardEffect_Ambassador(currentPlayer, cardToDiscard, copiesToDiscard, &state, 0);
#if (NOISY_TEST == 1)
    printf("cardEffect_Ambassador returned = %d, expected = -1\n", result);
#endif
    assertTrue(result == -1); // check if the return value is correct


#if (NOISY_TEST == 1)
    printf("Test player discarding more cards than they have.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.handCount[currentPlayer] = 2;
    state.hand[currentPlayer][0] = ambassador;
    state.hand[currentPlayer][1] = tribute;
    cardToDiscard = 1;
    copiesToDiscard = 2;
    result = cardEffect_Ambassador(currentPlayer, cardToDiscard, copiesToDiscard, &state, 0);
#if (NOISY_TEST == 1)
    printf("cardEffect_Ambassador returned = %d, expected = -1\n", result);
#endif
    assertTrue(result == -1); // check if the return value is correct


#if (NOISY_TEST == 1)
    printf("Test player discarding no cards.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.handCount[currentPlayer] = 2;
    state.hand[currentPlayer][0] = ambassador;
    state.hand[currentPlayer][1] = tribute;
    for (i = 1; i <=3; i++) {
      state.handCount[i] = 0;
    }
    cardToDiscard = 1;
    copiesToDiscard = 0;
    supplyCount = state.supplyCount[tribute];
    result = cardEffect_Ambassador(currentPlayer, cardToDiscard, copiesToDiscard, &state, 0);
#if (NOISY_TEST == 1)
    printf("cardEffect_Ambassador returned = %d, expected = 0\n", result);
#endif
    assertTrue(result == 0); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected tribute supplyCount = %d, actual tribute supplyCount = %d\n", supplyCount, state.supplyCount[tribute]);
#endif
    assertTrue(supplyCount == state.supplyCount[tribute]);

#if (NOISY_TEST == 1)
    printf("expected player 0 handCount = 1, actual handCount = %d\n",  state.handCount[currentPlayer]);
#endif
    assertTrue(state.handCount[currentPlayer] == 1);

    for (i = 1; i <=3; i++) {
#if (NOISY_TEST == 1)
        printf("expected player %d handCount = 0, actual handCount = %d\n", i, state.handCount[i]);
#endif
        assertTrue(state.handCount[i] == 0);
    }

#if (NOISY_TEST == 1)
    printf("Test player discarding one card.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game
    state.handCount[currentPlayer] = 2;
    state.hand[currentPlayer][0] = ambassador;
    state.hand[currentPlayer][1] = tribute;
    for (i = 1; i <=3; i++) {
      state.handCount[i] = 0;
    }
    cardToDiscard = 1;
    copiesToDiscard = 1;
    supplyCount = state.supplyCount[tribute];
    result = cardEffect_Ambassador(currentPlayer, cardToDiscard, copiesToDiscard, &state, 0);
#if (NOISY_TEST == 1)
    printf("cardEffect_Ambassador returned = %d, expected = 0\n", result);
#endif
    assertTrue(result == 0); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected tribute supplyCount = %d, actual tribute supplyCount = %d\n", supplyCount + 1, state.supplyCount[tribute]);
#endif
    assertTrue(supplyCount + 1 == state.supplyCount[tribute]);

#if (NOISY_TEST == 1)
    printf("expected player 0 handCount = 0, actual handCount = %d\n",  state.handCount[currentPlayer]);
#endif
    assertTrue(state.handCount[currentPlayer] == 0);

    for (i = 1; i <=3; i++) {
#if (NOISY_TEST == 1)
        printf("expected player %d handCount = 1, actual handCount = %d\n", i, state.handCount[i]);
#endif
        assertTrue(state.handCount[i] == 1);
#if (NOISY_TEST == 1)
        printf("expected player 1st card = tribute %d, actual 1st card = %d\n",  tribute, state.hand[i][0]);
#endif
        assertTrue(state.hand[i][0] == tribute);
    }


#if (NOISY_TEST == 1)
    printf("Test player discarding two cards.\n");
#endif
    memset(&state, 23, sizeof(struct gameState));   // clear the game state
    result = initializeGame(numPlayer, kingdoms, seed, &state); // initialize a new game      // set the number of cards on hand
    state.handCount[currentPlayer] = 3;
    state.hand[currentPlayer][0] = ambassador;
    state.hand[currentPlayer][1] = tribute;
    state.hand[currentPlayer][2] = tribute;
    for (i = 1; i <=3; i++) {
      state.handCount[i] = 0;
    }
    cardToDiscard = 1;
    copiesToDiscard = 2;
    supplyCount = state.supplyCount[tribute];
    result = cardEffect_Ambassador(currentPlayer, cardToDiscard, copiesToDiscard, &state, 0);
#if (NOISY_TEST == 1)
    printf("cardEffect_Ambassador returned = %d, expected = 0\n", result);
#endif
    assertTrue(result == 0); // check if the return value is correct

#if (NOISY_TEST == 1)
    printf("expected tribute supplyCount = %d, actual tribute supplyCount = %d\n", supplyCount + 2, state.supplyCount[tribute]);
#endif
    assertTrue(supplyCount + 2 == state.supplyCount[tribute]);

#if (NOISY_TEST == 1)
    printf("expected player 0 handCount = 0, actual handCount = %d\n",  state.handCount[currentPlayer]);
#endif
    assertTrue(state.handCount[currentPlayer] == 0);

    for (i = 1; i <=3; i++) {
#if (NOISY_TEST == 1)
        printf("expected player %d handCount = 2, actual handCount = %d\n", i, state.handCount[i]);
#endif
        assertTrue(state.handCount[i] == 2);
#if (NOISY_TEST == 1)
        printf("expected player 1st card = tribute %d, actual 1st card = %d\n",  tribute, state.hand[i][0]);
#endif
        assertTrue(state.hand[i][0] == tribute);
#if (NOISY_TEST == 1)
        printf("expected player 2nd card = tribute %d, actual 2nd card = %d\n",  tribute, state.hand[i][1]);
#endif
        assertTrue(state.hand[i][1] == tribute);
    }

    if (failedTests == 0) {
        printf("All %d tests passed!\n\n\n", tests);
    } else {
        printf("%d tests out of %d did not pass. That's a %d%% pass rate.\n\n\n", failedTests, tests, (int) (100 - failedTests * 100 / (float) tests));
    }

    return 0;
}