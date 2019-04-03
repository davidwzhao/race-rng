/*
 * Just a main function to test the rng!
 */

#include <stdio.h>
#include "rng.h"

int main(int argc, char **argv) {
    int num_hits[100] = {0};

    for (int i = 0; i < 100000; i++) {
        num_hits[rng() % 100] += 1;
    }

    for (int i = 0; i < 100; i++) {
        printf("%d, %d\n", i, num_hits[i]);
    }

    return 0;
}
