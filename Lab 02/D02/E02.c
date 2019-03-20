#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

// Global variables
int timeout = 0;
sem_t sem;

// Prototypes
void *th1routine(void *par);
void *th2routine(void *par);
int wait_with_timeout(sem_t *S, int tmax);

// Signal handler
static void handler(int sig) {
    if (sig == SIGALRM) {
        timeout = 1;
        sem_post(&sem);
    }
    return;
}

int main(int argc, char *argv[]) {

    int tmax;
    pthread_t tid[2];

    // Check arguments
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n\nUsage: %s tmax\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Create signal handler
    if (signal(SIGALRM, handler) == SIG_ERR) {
        fprintf(stderr, "Error SIGALRM.\n");
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

    int t;
    struct timespec req;

    // Set the seed for the rand function
    srand(time(NULL));

    // Generate t [1, 5]
    t = rand() % 5 + 1;
    req.tv_nsec = t * 1000000;
    fprintf(stdout, "Waiting on semaphore after %d milliseconds.\n", t);

    // Sleeping t milliseconds
    nanosleep(&req, NULL);

    // Wait no longer than tmax ms
    wait_with_timeout(&sem, *((int *) par));

    pthread_exit(NULL);
}

// Thread 2 routine
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

// Wait on semaphore S for a maximum of tmax milliseconds
int wait_with_timeout(sem_t *S, int tmax) {

    struct itimerval timer;

    // Setting the timer to tmax milliseconds
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = tmax/1000;
    timer.it_value.tv_usec = (tmax % 1000) * 1000;

    // Starting the timer
    if (setitimer(ITIMER_REAL, &timer, NULL) != 0) {
        fprintf(stderr, "Error setting timer.\n");
        exit(EXIT_FAILURE);
    }

    // Waiting on semaphore
    while (sem_wait(S) == -1) {
        if (errno == EINTR) {
            continue;
        } else {
            fprintf(stderr, "Error waiting semaphore.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Disarming timer
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &timer, NULL) != 0) {
        fprintf(stderr, "Error setting timer.\n");
        exit(EXIT_FAILURE);
    }

    // Display the correct message and return
    if (timeout) {
        fprintf(stderr, "Wait on semaphore s returned for timeout.\n");
        return 1;
    } else {
        printf("Wait returned normally.\n");
        return 0;
    }
}