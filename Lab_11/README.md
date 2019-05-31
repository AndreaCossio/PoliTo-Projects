# Instructions

## Exercise 01

A binary file stores the balance of all bank accounts held in a small bank branch. All the records are organized in the following way:

- An identifier
- A bank account number
- A surname (max 30 chars)
- A name (max 30 chars)
- The balance

The following is a correct example of such a file (in ASCII/text format):

    1 100000 Romano Antonio 1250
    2 150000 Fabrizi Aldo 2245
    3 200000 Verdi Giacomo 11115
    4 250000 Rossi Luigi 13630

Another binary file stores the operations done on bank accounts. The records in this file have the following format:

- An identifier
- A bank account number
- A surname (max 30 chars)
- A name (max 30 chars)
- Import of the operation (deposit / withdraw)

The following is a correct example of such a file (in ASCII/text format):

    1 100000 Romano Antonio +50
    3 200000 Verdi Giacomo +115
    1 100000 Romano Antonio +250
    1 100000 Romano Antonio -55
    3 200000 Verdi Giacomo -1015

Write a C program that satisfies the following specifications:

- The program receives ```N``` parameters on the command line. The first parameter specifies the name of an ```ACCOUNT``` file, while the others are all name of ```OPERATION``` files.
- The program has to open the ```ACCOUNT``` file and then run ```N-1``` threads (one for each ```OPERATION``` file).
- Each thread must read the operations specified on its ```OPERATION``` file and store the new balance of the account in the ```ACCOUNT``` file.
- When all threads have done their job, the resulting ```ACCOUNT``` file must be printed on standard output by the main process.

Write **4 versions** of the program:

- Version ```A```: mutual exclusion is guaranteed adopting file locking.
- Version ```B```: use critical sections to lock the entire ```ACCOUNT``` file.
- Version ```C```: same as version ```B```, but using mutexes.
- Version ```D```: same as version ```B```, but using semaphores.