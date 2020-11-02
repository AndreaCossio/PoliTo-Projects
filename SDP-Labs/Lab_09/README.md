# Instructions

## Exercise 01

A file stores integer values in ***binary*** format on 32 bits. The first integer stored in the file indicates the number of values following the first one. For example, the following three lines specify (obviously in ASCII format) the content of three possible files (that have to be stored in binary format):

    File 1: 5 23 45 67 9 23
    File 2: 12 90 65 34 24 12 0 89 29 54 12 78 3
    File 3: 3 9 5 1

Write a WIN 32 application which:

- Receives a variable number of strings, let us say ```n``` strings, on the command line. The first ```n-1``` strings are input file names. The last string is an output file name. Each input file has the format previously described.
- Runs one thread for each input file passing to each of them one of the input file names. We will refer to these ```n-1``` working threads as "ordering" threads.
- After running all ordering threads, the main application awaits for the termination of all of them.

When the main thread waits, each ordering thread:

- Opens "its own" input file
- Reads the first integer value
- Allocates a dynamic array of integers to store all other integer numbers stored in the file
- Read those numbers into the array
- Orders the array (in ascending order) using whatever ordering algorithm it is deemed appropriate
- Ends (returning the control to the main application thread).
- The main application thread, once collected the termination of all ordering threads, merges all ordered array into a unique array, i.e., a unique sequence of integers.
- It stores the resulting array into the output file, using the same format of the input files.

For the previous files the output file should be:

    20 0 1 3 5 9 9 12 15 23 23 24 29 34 45 54 65 67 78 89 90

## Exercise 02

A C program is run with two parameters ```name1``` ```name2```, which are C strings indicating relative or absolute paths of two directory trees.

The program must copy the directory ```name1``` into an isomorphic directory ```name2```.

## Exercise 03

A C program receives ```N``` parameters on the command line. Each parameter indicates a relative or an absolute path to a file system directory tree.

The program has to run ```N``` threads (one for each argument). Each thread recursively visits one of the directories, and, for each directory entry, it prints-out its thread identifier
and the directory entry name. The main thread awaits for the termination of all threads. When all threads have visited their directory also the program ends.

### Version A

As all output lines are generated independently by each single thread, printing messages from different threads are interleaved on standard output.

### Version B

To avoid the problem of Version A, Version B uses files. In other words, each thread stores its output on a file whose name is generated (to avoid conflicts) based on the thread identifier. When all working threads end, the main thread copies all files on standard output with the desired strategy (and no interruption).

### Version C - Optional / Elective

This version generalizes version B by using a centralized thread (the "output thread") dedicated to generate the entire application output.

All threads resort to the centralized and dedicated output thread to perform all operations on standard output. The output thread is executed within a critical section, and it just awaits for more output to come in, and then it takes care of it with the strategy at choice (i.e.,, it may store messages on files, like Version B, it may use a dedicated data structure to store messages, it may decide which verbosity level to use, it may compute output statistics, etc.).
