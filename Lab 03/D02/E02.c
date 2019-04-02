#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>

#define BUFLEN 16

// Custom condition struct
typedef struct Condition {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int count;
} Condition;

// Global variables
int in, out, buf[BUFLEN];
Condition empty, full;

// Prototypes
void sema_init(Condition *cond, int value);
void sema_wait(Condition *cond);
void sema_post(Condition *cond);
void produce(int data);
int consume();
void *producer(void *par);
void *consumer(void *par);

int main(int argc, char *argv[]) {

    pthread_t tid[2];

    // Initialize semaphores
    sema_init(&empty, BUFLEN);
    sema_init(&full, 0);

    in = 0;
    out = 0;

    // Create threads
    if (pthread_create(&(tid[0]), NULL, producer, 0) != 0 || pthread_create(&(tid[1]), NULL, consumer, 0) != 0) {
        perror("Error pthread_create()");
        exit(EXIT_FAILURE);
    }

    // Wait threads to finish
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}

// Custom initialization of semaphore using Conditions
void sema_init(Condition *cond, int value) {
    pthread_mutex_init(&(cond->lock), NULL);
    pthread_cond_init(&(cond->cond), NULL);
    cond->count = value;
}

// Custom wait of semaphore using Conditions
void sema_wait(Condition *cond) {
    pthread_mutex_lock(&(cond->lock));
    if (cond->count > 0) {
        cond->count--;
    } else {
        pthread_cond_wait(&(cond->cond), &(cond->lock));
    }
    pthread_mutex_unlock(&(cond->lock));
}

// Custom post of semaphore using Conditions
void sema_post(Condition *cond) {
    pthread_mutex_lock(&(cond->lock));
    if (cond->count == 0) {
        pthread_cond_signal(&(cond->cond));
    }
    cond->count++;
    pthread_mutex_unlock(&(cond->lock));
}

// Write the new element in the buffer
void produce(int data) {
    sema_wait(&empty);
    buf[in] = data;
    in = (in + 1) % BUFLEN;
    sema_post(&full);
}

// Read the new element from the buffer
int consume() {
    int data;
    sema_wait(&full);
    data = buf[out];
    out = (out + 1) % BUFLEN;
    sema_post(&empty);
    return data;
}

// Producer routine
void *producer(void *par) {
    
    int i;

    // Main loop producing integers [0, 9999]
    for (i = 0; i < 10000; i++) {
        produce(i);
    }

    // Make the consumer exit
    produce(-1);

    pthread_exit(NULL);
}

// Consumer routine
void *consumer(void *par) {

    int data = 0;

    // Consume and print integers
    while (data != -1) {
        data = consume();
        printf("Received %d\n", data);
    }

    pthread_exit(NULL);
}