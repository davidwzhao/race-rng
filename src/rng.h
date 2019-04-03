#ifndef RNG_H
#define RNG_H

/* ----- struct definitions ----- */

/* Parameters for an rng function */
typedef struct rng_params_t_ {
    unsigned long long a;
    unsigned long long b;
} rng_params_t;

/* Arguments passed into the loop rng function */
typedef struct loop_f_args_t_ {
    // a pointer to the random number
    unsigned long long *num;

    // the parameters for the function
    rng_params_t *rng_params;

    // the number of loops
    int num_loops;

    // the rng function to call - paramters match rng_f
    void (*fn) (unsigned long long *, rng_params_t *rng_params);
} loop_f_args_t;

/* Configuration for the rng */
typedef struct rng_config_t_ {
    // number of threads
    int num_threads;

    // number of loops for each thread
    int num_loops;

    // starting random number
    unsigned long long init_random;

    // array of rng parameters, should be 1 per thread
    rng_params_t *rng_params;
} rng_config_t;

/* ----- function forward declarations ----- */

/* 
 * The main rng function. The default function is a simple
 * Fibonacci-style PRNG: 
 * n = n + (a >> 32) + b
 * b = a
 * a = n
 *
 * May be changed if you define your own function
 * with the same parameters
 *
 * @param num a pointer to the random number to be modified
 * @param rng_params the parameters to use for this PRNG
 * @param thread_id a number identifying a thread, usually 0, 1, 2, etc.
 */
void rng_f(unsigned long long *num, rng_params_t *rng_params);

/* 
 * Loops the rng function
 *
 * @param args an element of type loop_f_args_t defining arguments
 */
void *loop_f(void *args);

/*
 * The main rng function with a config
 *
 * @return the random number
 * @param config the configuration for this rng
 */
unsigned long long rng_config(rng_config_t *config);

/*
 * The main rng function - call this to make a new random number!
 * Calls rng(rng_config_t) with a default config
 *
 * @return the random number
 */
unsigned long long rng();

#endif /* RNG_H */
