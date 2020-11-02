#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

void *pRoutine(void *par);
void *oRoutine(void *par);

int main(int argc, char *argv[]) {

    int loop_count = 0;
    char next = ' ', this = ' ', last = ' ';
    pthread_t tid[2];
    FILE *fp;

    // Check arguments
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n\nUsage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open and check file
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error opening file %s.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Read the file one char at a time
    while (fscanf(fp, "%c", &next) != EOF) {

        /* From the third iteration */
        if (loop_count > 1) {
            // Wait the Output thread
            pthread_join(tid[1], NULL);
        }

        /* From the second iteration */
        if (loop_count > 0) {
            // Wait the Processing thread
            pthread_join(tid[0], NULL);
            // Copy 'this' into 'last'
            last = this;
            // Create the Output thread
            pthread_create(&tid[1], NULL, oRoutine, &last);
        }

        /* Always */
        // Copy 'next' into 'this'
        this = next;
        // Create the Processing thread
        pthread_create(&tid[0], NULL, pRoutine, &this);
        
        loop_count++;
    }

    // Wait threads
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    // Output the last char with an Output thread
    last = this;
    pthread_create(&tid[1], NULL, oRoutine, &last);
    pthread_join(tid[1], NULL);

    return 0;
}

// Processing thread routine
void *pRoutine(void *par) {
    *((char *)par) = toupper(*((char *)par));
    pthread_exit(NULL);
}

// Output thread routine
void *oRoutine(void *par) {
    printf("%c", *((char *)par));
    pthread_exit(NULL);
}