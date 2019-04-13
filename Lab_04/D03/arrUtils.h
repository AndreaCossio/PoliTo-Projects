#ifndef ARRUTIL_H
#define ARRUTIL_H

float *generateArray(int n);
float **generateMatrix(int rows, int cols);
void printArray(float *arr, int n);
void printMatrix(float **mat, int rows, int cols);
float *prodMatArr(float **mat, int rows, int cols, float *arr, int n);
float prodArrArr(float *v1, float *v2, int n);

#endif