#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "quicksort.h"

void generateArray(int *arr, int n, int range, int offset);
void writeArrayText(int *arr, int n, char *filename);
void writeArrayBin(int *arr, int n, char *filename);

int main(int argc, char *argv[]) {
    
    int n1, n2, *v1, *v2;

    // Check arguments
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments.\n\nUsage: %s n1 n2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse arguments
    n1 = atoi(argv[1]);
    n2 = atoi(argv[2]);

    // Allocate arrays
    v1 = (int *) malloc(n1 * sizeof(int));
    v2 = (int *) malloc(n2 * sizeof(int));

    // Generate arrays and sort them
    generateArray(v1, n1, 45, 10);
    generateArray(v2, n2, 40, 21);
    qsWrapper(v1, n1);
    qsWrapper(v2, n2);

    // Write text files
    writeArrayText(v1, n1, "fv1.txt");
    writeArrayText(v2, n2, "fv2.txt");

    // Write binary files
    writeArrayBin(v1, n1, "fv1.b");
    writeArrayBin(v2, n2, "fv2.b");

    // Memory deallocation
    free(v1);
    free(v2);

    return 0;
}

// Generate an array of size n given a range for even numbers and an offset
void generateArray(int *arr, int n, int range, int offset) {

    int i;

    // Check memory allocation
    if (arr == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }

    // Set the seed for the rand function
    srand(time(NULL));

    // Generate values
    for (i = 0; i < n; i++) {
        arr[i] = (rand() % (range+1))*2 + offset;
    }

    return;
}

// Write an array of size n inside a text file with given filename
void writeArrayText(int *arr, int n, char *filename) {

    int i, fd;
    char *s = malloc(5*sizeof(char));

    // Open and check file
    if ((fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0) {
        fprintf(stderr, "Error opening file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    // Write the array
    for (i = 0; i < n; i++) {
        sprintf(s, "%d ", arr[i]);
        if (write(fd, s, strlen(s)*sizeof(char)) != strlen(s)) {
            fprintf(stderr, "Error writing the file %s.\n", filename);
            exit(EXIT_FAILURE);
        }
    }
    write(fd, "\n", sizeof(char));

    // Close file
    close(fd);

    return;
}

// Write an array of size n inside a text file with given filename
void writeArrayBin(int *arr, int n, char *filename) {
    
    int fd;

    // Open and check file
    if ((fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0) {
        fprintf(stderr, "Error opening file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    // Write the array and check
    if (write(fd, arr, n*sizeof(int)) != n*sizeof(int)) {
        fprintf(stderr, "Error writing the file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    // Close file
    close(fd);

    return;
}