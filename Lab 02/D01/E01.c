#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>

// Global variables
int g, finish = 2, counter = 0;
sem_t *multiply, *print, *mutex;

// Prototypes
void *routine(void *par);

// Signal handler
static void handler(int sig) {
    if (--finish == 0) {
        printf("Total served requests: %d\n", counter);
        exit(EXIT_SUCCESS);
    }
    return;
}

int main(int argc, char *argv[]) {

    pthread_t tid[2];

    // Create signal handler
    if (signal(SIGUSR1, handler) == SIG_ERR) {
        fprintf(stderr, "Error SIGUSR1.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate semaphores
    multiply = (sem_t *) malloc(sizeof(sem_t));
    print = (sem_t *) malloc(sizeof(sem_t));
    mutex = (sem_t *) malloc(sizeof(sem_t));

    // Check allocation
    if (multiply == NULL || print == NULL || mutex == NULL) {
        fprintf(stderr, "Error allocating semaphores.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphores
    sem_init(multiply, 0, 0);
    sem_init(print, 0, 0);
    sem_init(mutex, 0, 1);

    // Create threads
    if (pthread_create(&(tid[0]), NULL, routine, "fv1.b") != 0 || pthread_create(&(tid[1]), NULL, routine, "fv2.b") != 0) {
        fprintf(stderr, "Error creating thread.\n");
        exit(EXIT_FAILURE);
    }

    // Main loop waiting on the semaphore
    while (1) {
        sem_wait(multiply);
        g *= 3;
        counter ++;
        sem_post(print);
    }

    return 0;
}

// Thread routine
void *routine(void *par) {

    int x, fd;
    char *filename = (char *) par;

    // Open and check file
    if ((fd = open(filename, O_RDONLY, 0644)) < 0) {
        fprintf(stderr, "Error opening file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    // Read loop
    while (read(fd, &x, sizeof(int)) == sizeof(int)) {
        // Mutex section
        sem_wait(mutex);

        g = x;

        // Signal and wait
        sem_post(multiply);
        sem_wait(print);

        // Print identifier and result
        printf("ThreadID: %ld\t%d\n", pthread_self(), g);

        // End of mutex section
        sem_post(mutex);
    }

    // Send finish signal
    raise(SIGUSR1);

    // Close file
    close(fd);

    pthread_exit(NULL);
}