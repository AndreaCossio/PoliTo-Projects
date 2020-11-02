#include <stdio.h>
#include <stdlib.h>
#include "arrUtils.h"

float myRand(float offset) {
    return (float) rand() / RAND_MAX - offset;
}

float *generateArray(int n) {

    int i;
    float *arr;
    
    // Allocate and generate array
    arr = (float *) malloc(n*sizeof(float));
    for (i = 0; i < n; i++) {
        arr[i] = myRand(0.5);
    }

    return arr;
}

float **generateMatrix(int rows, int cols) {

    int i;
    float **mat;

    // Allocate and generate matrix
    mat = (float **) malloc(rows*sizeof(float *));
    for (i = 0; i < rows; i++) {
        mat[i] = generateArray(cols);
    }

    return mat;
}

void printArray(float *arr, int n) {
    
    int i;

    // Print array
    for (i = 0; i < n; i++) {
        printf("%f ", arr[i]);
    }

    printf("\n");

    return;
}

void printMatrix(float **mat, int rows, int cols) {
    
    int i;

    // Print array
    for (i = 0; i < rows; i++) {
        printArray(mat[i], cols);
    }

    return;
}

float *prodMatArr(float **mat, int rows, int cols, float *arr, int n) {
    
    int i;
    float *result;

    if (cols != n) {
        printf("The number of columns of the matrix must be equal to the number of rows of the array.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate result
    result = (float *) malloc(rows*sizeof(float));

    // Product
    for (i = 0; i < rows; i++) {
        result[i] = prodArrArr(mat[i], arr, n);
    }

    return result;
}

float prodArrArr(float *v1, float *v2, int n) {

    int i;
    float x = 0;

    // Product
    for (i = 0; i < n; i++) {
        x += v1[i] * v2[i];
    }

    return x;
}