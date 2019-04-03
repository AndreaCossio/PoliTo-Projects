#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

#define BUFLEN 16

// Global variables
sem_t urgempty, urgfull, normempty, normfull;
int urgin, urgout, normin, normout, nurg, nnorm;
long long urgbuf[BUFLEN], normbuf[BUFLEN];

// Prototypes
void produce(int urgent, long long data);
int consume(int urgent);
void *producer(void *par);
void *consumer(void *par);
long long current_timestamp();

int main(int argc, char *argv[]) {

    pthread_t tid[2];

    // Initialize semaphores
    sem_init(&normempty, 0, BUFLEN);
    sem_init(&normfull, 0, 0);
    sem_init(&urgempty, 0, BUFLEN);
    sem_init(&urgfull, 0, 0);

    normin = 0;
    normout = 0;
    urgin = 0;
    urgout = 0;
    nnorm = 0;
    nurg = 0;

    // Create threads
    if (pthread_create(&(tid[0]), NULL, producer, 0) != 0 || pthread_create(&(tid[1]), NULL, consumer, 0) != 0) {
        perror("Error pthread_create()");
        exit(EXIT_FAILURE);
    }

    // Wait threads to finish
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    // Print statistic
    printf("Normal buffer percentage: %f\n", (100.0*nnorm)/(nurg+nnorm));

    return 0;
}

// Write the new element in the buffer
void produce(int urgent, long long data) {
    if (urgent) {
        while (sem_wait(&urgempty) == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("Error sem_wait()");
                exit(EXIT_FAILURE);
            }
        }
        printf("Putting %llu in the urgent buffer.\n", data);
        urgbuf[urgin] = data;
        urgin = (urgin + 1) % BUFLEN;
        sem_post(&urgfull);
    } else {
        while (sem_wait(&normempty) == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("Error sem_wait()");
                exit(EXIT_FAILURE);
            }
        }
        if (data != -1) {
            printf("Putting %llu in the normal buffer.\n", data);
        }
        normbuf[normin] = data;
        normin = (normin + 1) % BUFLEN;
        sem_post(&normfull);
    }
}

// Read the new element from the buffer
int consume(int urgent) {
    if (urgent) {
        printf("Received %llu from the urgent buffer.\n", urgbuf[urgout]);
        urgout = (urgout + 1) % BUFLEN;
        sem_post(&urgempty);
    } else {
        if (normbuf[normout] == -1) {
            return -1;
        }
        printf("Received %llu from the normal buffer.\n", normbuf[normout]);
        normout = (normout + 1) % BUFLEN;
        sem_post(&normempty);
    }
    return 0;
}

// Producer routine
void *producer(void *par) {
    
    int i, t;
    long long ms;
    struct timespec req;

    // Set the seed for the rand function
    srand(time(NULL));

    // Main loop
    for (i = 0; i < 10000; i++) {

        // Generate t [1, 10]
        t = rand() % 10 + 1;
        req.tv_nsec = t * 1000000;

        // Sleeping t milliseconds
        nanosleep(&req, NULL);

        ms = current_timestamp();

        // Critical section
        if (rand() % 100 > 80) {
            produce(1, ms);
            nurg++;
        } else {
            produce(0, ms);
            nnorm++;
        }
    }

    // Make the consumer exit
    produce(0, -1);

    pthread_exit(NULL);
}

// Consumer routine
void *consumer(void *par) {

    int ret;
    struct timespec req;

    // Set up 10 ms
    req.tv_nsec = 10 * 1000000;

    while (1) {

        // Sleep 10 ms
        nanosleep(&req, NULL);

        // Try urgent buffer
        while ((ret = sem_trywait(&urgfull)) == -1) {
            if (errno == EINTR) {
                continue;
            } else if (errno = EAGAIN) {
                break;
            } else {
                perror("Error sem_trywait()");
                exit(EXIT_FAILURE);
            }
        }

        // If the urgent buffer had something
        if (ret == 0) {
            // Consume from urgent
            consume(1);
            // Restart the loop
            continue;
        }

        // Try normal buffer
        while ((ret = sem_trywait(&normfull)) == -1) {
            if (errno == EINTR) {
                continue;
            } else if (errno = EAGAIN) {
                break;
            } else {
                perror("Error sem_trywait()");
                exit(EXIT_FAILURE);
            }
        }

        // If the normal buffer had something
        if (ret == 0) {
            // Consume from normal buffer anc check exit
            if (consume(0) == -1) {
                pthread_exit(NULL);
            }
        }
    }
}

// Return the current_timestamp in milliseconds
long long current_timestamp() {

    struct timeval te;

    gettimeofday(&te, NULL);

    return te.tv_sec*1000LL + te.tv_usec/1000;
}