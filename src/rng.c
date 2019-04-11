#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rng.h"

// barrier used to ensure all threads start at the same time
pthread_barrier_t barr;

void rng_f(uint64_t *num, rng_params_t *rng_params) {
    *num ^= (*num << 11);
    *num ^= (*num >> 8);
    *num ^= rng_params->b ^ (rng_params->b >> 19) + (rng_params->a >> 26);

    rng_params->b = (rng_params->a >> 4);
    rng_params->a = (*num >> 3);
}

void *loop_f(void *a) {
    loop_f_args_t *args = a;

    // all threads wait on barrier so they start at the same time
    pthread_barrier_wait(&barr);

    // main loop for the rng
    for (int i = 0; i < args->num_loops; i++) {
        args->fn(args->num, args->rng_params);
    }
    
    // needs to return NULL for pthreads
    return NULL;
}

uint64_t rng_config(rng_config_t *config) {
    const int num_threads = config->num_threads;
    uint64_t random = config->init_random;

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

uint64_t rng() {
    // make sure this has conf.num_threads number of elements
    rng_params_t rng_params[] = {
        { .a = 1959145553,          .b = 6983543408444742683    },
        { .a = 1837103774,          .b = 1629451259722584379    },
        { .a = 8411011023567752081, .b = 7948111962147686778    },
        { .a = 3554592823790559842, .b = 5247505673804591734    }
    };

    rng_config_t conf = {
        .num_threads = 4,
        .num_loops = 100,
        .init_random = 1,
        .rng_params = rng_params
    };

    return rng_config(&conf);
}
