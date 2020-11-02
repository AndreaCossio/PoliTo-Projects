#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

/* Na = 0, Cl = 1 */

typedef struct {
    int id, k;
} targ_t;

typedef struct {
    int id, atom;
} atom_t;

int *ids[2], count[2];
pthread_mutex_t me[2];
sem_t clReady, naFinish;

void *routine(void *arg);
void *atomRoutine(void *arg);

int main(int argc, char *argv[]) {

    int i, k;
    pthread_t tids[2];
    targ_t ts[2];

    // Check and parse args
    if (argc != 2) {
        fprintf(stderr, "Usage: %s k\n", argv[0]);
        return -1;
    }
    k = atoi(argv[1]);

    // Init variables
    for (i = 0; i < 2; i++) {
        ids[i] = (int*) malloc(k * sizeof(int));
        count[i] = 0;
        pthread_mutex_init(&(me[i]), NULL);
    }
    sem_init(&clReady, 0, 0);
    sem_init(&naFinish, 0, 0);

    // Set seed
    srand(time(NULL));

    // Create the 2 threads
    for (i = 0; i < 2; i++) {
        ts[i].id = i;
        ts[i].k = k;
        pthread_create(&(tids[i]), NULL, routine, &(ts[i]));
    }

    // Wait them
    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);

    // Free variables
    for (i = 0; i < 2; i++) {
        free(ids[i]);
        pthread_mutex_destroy(&(me[i]));
    }
    sem_destroy(&clReady);
    sem_destroy(&naFinish);

    return 0;
}

void *routine(void *arg) {
    
    int i;
    pthread_t *tids;
    atom_t *atoms;
    targ_t *t = (targ_t *)arg;

    // Allocate atom thread resources
    tids = (pthread_t*)malloc(t->k * sizeof(pthread_t));
    atoms = (atom_t*)malloc(t->k * sizeof(atom_t));

    // Generate atoms
    for (i = 0; i < t->k; i++) {
        atoms[i].id = i + t->k * t->id;
        atoms[i].atom = t->id;
        pthread_create(&(tids[i]), NULL, atomRoutine, &(atoms[i]));
    }

    // Wait for threads
    for (i = 0; i < t->k; i++) {
        pthread_join(tids[i], NULL);
    }

    // Free resources
    free(tids);
    free(atoms);

    pthread_exit(NULL);
}

void *atomRoutine(void *arg) {

    atom_t a = *(atom_t *)arg;
    int type = a.atom;

    // Creation time
    sleep(rand() % 4);

    // Enter mutex
    pthread_mutex_lock(&(me[type]));
    
    // Write id in the array
    ids[type][count[type]] = a.id;
    
    // Na
    if (type == 0) {
        sem_wait(&clReady);
        printf("id: %d - Na %d Cl %d ", a.id, a.id, ids[1][count[type]]);
        sem_post(&naFinish);
    }
    // Cl
    else {
        sem_post(&clReady);
        sem_wait(&naFinish);
        printf("id: %d - Na %d Cl %d\n", a.id, ids[0][count[type]], a.id);
    }

    // Increment counter and leave mutex
    count[type]++;
    pthread_mutex_unlock(&(me[type]));

    pthread_exit(NULL);
}