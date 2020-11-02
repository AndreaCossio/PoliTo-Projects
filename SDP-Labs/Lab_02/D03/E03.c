#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

// Global semaphore
sem_t sem;

// Prototypes
void *th1routine(void *par);
void *th2routine(void *par);

int main(int argc, char *argv[]) {

    int tmax;
    pthread_t tid[2];

    // Check arguments
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n\nUsage: %s tmax\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse tmax
    tmax = atoi(argv[1]);

    // Initialize semaphore
    sem_init(&sem, 0, 0);

    // Create threads
    if (pthread_create(&(tid[0]), NULL, th1routine, &tmax) != 0 || pthread_create(&(tid[1]), NULL, th2routine, NULL) != 0) {
        fprintf(stderr, "Error creating thread.\n");
        exit(EXIT_FAILURE);
    }

    // Wait only the first thread
    pthread_join(tid[0], NULL);

    return 0;
}

// Thread 1 routine
void *th1routine(void *par) {

    int t, tmax;
    struct timespec req, ts;

    tmax = *((int *) par);

    // Set the seed for the rand function
    srand(time(NULL));

    // Generate t [1, 5]
    t = rand() % 5 + 1;
    req.tv_nsec = t * 1000000;
    printf("Waiting on semaphore after %d milliseconds.\n", t);

    // Sleeping t milliseconds
    nanosleep(&req, NULL);

    // Get current time
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        fprintf(stderr, "Error getting real time.\n");
        exit(EXIT_FAILURE);
    }

    // Setting the timer to tmax milliseconds from now
    ts.tv_sec += tmax/1000;
    ts.tv_nsec += (tmax % 1000) * 1000000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec += 1;
        ts.tv_nsec -= 1000000000;
    }
    
    // Wait no longer than tmax ms
    while (sem_timedwait(&sem, &ts) == -1) {
        if (errno == EINTR) {
            continue;
        } else if (errno == ETIMEDOUT) {
            fprintf(stderr, "Wait on semaphore s returned for timeout.\n");
            exit(EXIT_FAILURE);
        } else {
            fprintf(stderr, "Error waiting semaphore.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Wait returned normally.\n");

    pthread_exit(NULL);
}

void *th2routine(void *par) {

    int t;
    struct timespec req;

    // Set the seed for the rand function
    srand(time(NULL));

    // Generate t [1000, 10000]
    t = rand() % 9001 + 1000;
    req.tv_sec = t/1000;
    req.tv_nsec = (t % 1000) * 1000000;
    fprintf(stdout, "Performing signal on semaphore after %d milliseconds.\n", t);

    // Sleeping t milliseconds
    nanosleep(&req, NULL);

    sem_post(&sem);

    pthread_exit(NULL);
}