# Instructions

## Exercise 01

Optimize [Exercise 01 of Laboratory 09](../Lab_09/README.md#exercise-01) as follows: the main thread, instead of waiting for the termination of all ordering threads *before* merging their results, it starts merging ordered sequences every time one ordering thread finishes.

In other words the main thread:

- Runs all ordering threads
- As soon as one ordering thread terminates, it merges the ordered sequence coming from this thread with its own ordered sequence (initially empty). It generates a unique *new* ordered sequence.
- It proceeds until all ordering threads have terminated. At this point the generated sequence is the desired one.
- It stores the final sequence into the output file.

## Exercise 02

A C program is run with `N` parameters, each indicating a relative or an absolute path to a directory. The program has to compare the content of all directories to decide whether they have the same content or not. Two directories have the same content if all the entries (files and sub-directories) have the same name (excluding the path leading to the root). Upon termination the program has to state whether all directory trees have the same content or not.
