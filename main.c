#include <stdio.h>
#include "rng.h"

int main(int argc, char **argv) {
    for (int i = 0; i < 100; i++) {
        printf("%lld\n", rng() /*% 100*/);
    }

    return 0;
}
