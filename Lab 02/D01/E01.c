#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>

// Global variables
int g;
sem_t s[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *par);

int main(int argc, char *argv[]) {

    int finish = 2, counter = 0;
    pthread_t tid[2];

    // Initialize semaphores
    sem_init(&s[0], 0, 0);
    sem_init(&s[1], 0, 0);

    // Create threads
    pthread_create(&(tid[0]), NULL, routine, "fv1.b");
    pthread_create(&(tid[1]), NULL, routine, "fv2.b");

    // Main loop waiting on the semaphore
    while (finish > 0) {
        sem_wait(&s[0]);
        if (g == -1) {
            finish--;
        } else {
            g *= 3;
            counter ++;
        }
        sem_post(&s[1]);
    }

    // Wait threads
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    // Print stats
    printf("Total requests: %d\n", counter);

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
        pthread_mutex_lock(&mutex);

        g = x;

        // Signal and wait
        sem_post(&s[0]);
        sem_wait(&s[1]);

        // Print identifier and result
        fprintf(stdout, "%ld: %d\n", pthread_self(), g);

        // End of mutex section
        pthread_mutex_unlock(&mutex);
    }

    // Set g = -1 to finish
    pthread_mutex_lock(&mutex);
    g = -1;
    sem_post(&s[0]);
    sem_wait(&s[1]);
    pthread_mutex_unlock(&mutex);

    // Close file
    close(fd);

    pthread_exit(NULL);
}