#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "arrUtils.h"

// Thread params
typedef struct params {
    int i, k;
    float *v1, *v2, *v, **mat, *result;
} params_t;

// Counter
typedef struct counter {
    int count;
    pthread_mutex_t mutex;
} counter_t;

// Global variables
counter_t *c;

// Prototypes
void runWithoutThreads(float *v1, float *v2, float **mat, int k);
void runWithThreads(float *v1, float *v2, float **mat, int k);
void *routine(void *args);
params_t *fillStruct(float *v1, float *v2, float **mat, float *v, float *result, int k, int i);

int main(int argc, char *argv[]) {

    int k;
    float *v1, *v2, **mat;

    // Check arguments
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n\nUsage: %s k\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse argument
    k = atoi(argv[1]);

    // Set the seed for the rand function
    srand(time(NULL));

    // Generate arrays and matrix
    v1 = generateArray(k);
    v2 = generateArray(k);
    mat = generateMatrix(k, k);

    // Non threaded version
    runWithoutThreads(v1, v2, mat, k);

    // Threaded version
    runWithThreads(v1, v2, mat, k);

    // Free resources
    free(v1);
    free(v2);
    free(mat);

    return 0;
}

// Non-threaded version of the product
void runWithoutThreads(float *v1, float *v2, float **mat, int k) {

    float *v, result;

    // Mat * v2
    v = prodMatArr(mat, k, k, v2, k);
    
    // v1 * v
    result = prodArrArr(v1, v, k);

    // Print result
    printf("The result is: %.4f\n", result);

    // Free resources
    free(v);

    return;
}

// Threaded version of the product
void runWithThreads(float *v1, float *v2, float **mat, int k) {

    int i;
    float result, *v;
    params_t **ps;
    pthread_t *tids;

    // Allocations
    v = (float *) malloc(k*sizeof(float));
    tids = (pthread_t *) malloc(k*sizeof(pthread_t));
    ps = (params_t **) malloc(k*sizeof(params_t *));
    c = (counter_t *) malloc(sizeof(counter_t));

    // Init counter that will work as barrier
    c->count = k;
    pthread_mutex_init(&(c->mutex), NULL);

    // Create threads
    for (i = 0; i < k; i++) {
        ps[i] = fillStruct(v1, v2, mat, v, &result, k, i);
        pthread_create(&(tids[i]), NULL, routine, (void *) ps[i]);
    }

    // Wait threads
    for (i = 0; i < k; i++) {
        pthread_join(tids[i], NULL);
        free(ps[i]);
    }

    // Print result
    printf("The result is: %.4f\n", result);

    // Free resources
    free(v);
    free(tids);
    free(ps);
    free(c);

    return;
}

// Threads' routine
void *routine(void *args) {

    params_t *par = (params_t *) args;

    // i-th row times v2
    (par->v)[par->i] = prodArrArr(par->mat[par->i], par->v2, par->k);

    // Decrement counter
    pthread_mutex_lock(&(c->mutex));

    c->count--;

    // If last thread generate result
    if (c->count == 0) {
        *(par->result) = prodArrArr(par->v1, par->v, par->k);
    }

    pthread_mutex_unlock(&(c->mutex));

    pthread_exit(NULL);
}

// Write params inside a struct
params_t *fillStruct(float *v1, float *v2, float **mat, float *v, float *result, int k, int i) {

    params_t *par;

    par = (params_t *) malloc(sizeof(params_t));

    par->v1 = v1;
    par->v2 = v2;
    par->mat = mat;
    par->v = v;
    par->result = result;
    par->k = k;
    par->i = i;

    return par;
}