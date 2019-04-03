#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rng.h"

// barrier used to ensure all threads start at the same time
pthread_barrier_t barr;

void rng_f(unsigned long long *num, rng_params_t *rng_params) {
    *num += rng_params->a + rng_params->b;
    rng_params->b = rng_params->a;
    rng_params->a = *num;
}

void *loop_f(void *a) {
    loop_f_args_t *args = a;

    // all threads wait on barrier so the start at the same time
    pthread_barrier_wait(&barr);

    // main loop for the rng
    for (int i = 0; i < args->num_loops; i++) {
        args->fn(args->num, args->rng_params);
    }
    
    // needs to return NULL for pthreads
    return NULL;
}

unsigned long long rng_config(rng_config_t *config) {
    const int num_threads = config->num_threads;
    unsigned long long random = config->init_random;

    pthread_t threads[num_threads];

    loop_f_args_t args[num_threads];

    if (pthread_barrier_init(&barr, NULL, num_threads)) {
        fprintf(stderr, "error initializing barrier");
        exit(1);
    }

    for (int i = 0; i < num_threads; i++) {
        args[i].num = &random;
        args[i].rng_params = &config->rng_params[i];
        args[i].num_loops = config->num_loops;
        args[i].fn = &rng_f;

        if (pthread_create(&(threads[i]), NULL, &loop_f, (void *) &args[i])) {
            fprintf(stderr, "error creating thread");
            exit(1);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "error joining threads");
            exit(1);
        }
    }

    return random;
}

unsigned long long rng() {
    // make sure this has conf.num_threads number of elements
    rng_params_t rng_params[] = {
        { .a = 12398,           .b = 18472621378        },
        { .a = 198472138974,    .b = 234879             },
        { .a = 19284397,        .b = 13628747327846     },
        { .a = 938443,          .b = 5978247            }
    };

    rng_config_t conf = {
        .num_threads = 4,
        .num_loops = 1000,
        .init_random = 1943867283,
        .rng_params = rng_params
    };

    return rng_config(&conf);
}
