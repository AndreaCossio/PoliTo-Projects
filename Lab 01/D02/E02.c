#include <unistd.h>
#include <sys/wait.h>
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

    int n, *v, pid[2], i, status, error;

    // Check arguments
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments.\n\nUsage: %s n1 n2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Generate two children
    for (i = 0; i < 2; i++) {
        if ((pid[i] = fork()) == 0) {

            // Parse argument and allocate array
            n = atoi(argv[i+1]);
            v = malloc(n * sizeof(int));

            // Generate array and sort
            if (i == 0) {
                generateArray(v, n, 45, 10);
            } else {
                generateArray(v, n, 40, 21);
            }
            qsWrapper(v, n);

            // Write text and binary file
            if (i == 0) {
                writeArrayText(v, n, "fv1.txt");
                writeArrayBin(v, n, "fv1.b");
            } else {
                writeArrayText(v, n, "fv2.txt");
                writeArrayBin(v, n, "fv2.b");
            }

            // Memory deallocation
            free(v);

            // Exit
            exit(i+1);
        }
    }

    // Wait children and catch their exit status
    for (i = 0; i < 2; i++) {
        waitpid(pid[i], &status, 0);
        if(WEXITSTATUS(status) == i+1) {
            printf("Child %d exited correctly; exit value: %d.\n", pid[i], WEXITSTATUS(status));
        } else {
            printf("Child %d terminated with error; exit value: %d.\n", pid[i], WEXITSTATUS(status));
            error = 1;
        }
    }

    // Exit
    if (error)
        exit(EXIT_FAILURE);
    else
        exit(EXIT_SUCCESS);
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