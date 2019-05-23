# Instructions

## Exercise 03

Implement a sequential program in C that takes a single argument ```k``` from the command line. The program creates two vectors ```v1``` and ```v2``` of dimension ```k```, and a matrix ```mat``` of dimension ```k*k```, all filled with random numbers in the range ```[-0.5 0.5]```, then it performs the product <code>v1<sup>T</sup> * mat * v2</code>, and print the result.

Perform the product operation in two steps: ```v = mat * v2```, which produces a new vector ```v```, and <code>result = v1<sup>T</sup> * v</code>. Then, write a concurrent program using threads that performs the same task. The main thread creates the vectors, the matrix, and ```k``` threads. Then, it waits the termination of the other threads. Each thread performs the product of the ```i-th``` row vector of ```mat``` and ```v2```, which produces the ```i-th``` element of vector ```v```. One of the created threads, the last terminating its product operation, performs the final operation <code>result = v1<sup>T</sup> * v</code>, and prints the result.