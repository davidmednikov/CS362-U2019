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