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

## Exercise 02

Realize in the Windows environment a producer/consumer application with the following characteristics:

- There are ```P``` producers and ```C``` consumers.
- Producers and consumers communicate using a circular buffer. The circular buffer is implemented as a queue on a dynamic array of size ```N```.
- Each producer inserts in the queue integer values (randomly generated) at random time intervals (randomly generated and varying from ```0``` to ```T``` seconds).
- Each consumer extracts from the queue an integer value at random time intervals (randomly generated and varying from ```0``` to ```T``` seconds) and prints it out on standard output with its personal thread identifier.

Note that:

- The integer values ```P```, ```C```, ```N```, and ```T``` are passed to the program on the command line.
- The circular queue has to be properly protected, to avoid:
  - insertions in a full queue.
  - extractions from an empty queue.
  - insertions by two producers at the same time.
  - extractions by two consumers at the same time.
- Find a reasonable way to terminate all producers and all consumers.

## Exercise 03

A one-way bridge is approached by cars in both directions, but being one-way it must follow three rules:

- Only cars from one direction can traverse the bridge in each moment, either the ones moving from ```left-to-right``` or the one running from ```right-to-left```.
- There can be multiple cars going in the same direction.
- If cars from the opposite direction arrive while there are cars on the bridge, those cars in the opposite direction must wait till the bridge is cleared.

Write a Windows program to synchronize all cars traversing the bridge. More specifically, the program must run two threads. The first thread is in charge of all cars moving from ```left-to-right```. The second one is in charge of all cars moving from ```right-to-left```. Each one of the previous two threads run one extra thread to represent each car.

Notice that:

- Cars running from ```left-to-right``` arrive at random time intervals, varying from ```0``` to ```time_A_L2R``` (integer value) seconds.
- Cars running from ```right-to-left``` arrive at random time intervals, varying from ```0``` to ```time_A_R2L``` (integer value) seconds.
- All cars traversing the bridge from ```left-to-right``` need ```time_T_L2R``` (integer value) seconds.
- All cars traversing the bridge from ```right-to-left``` need ```time_T_R2L``` (integer value) seconds.
- The program has to terminate only once exactly
  - ```number_L2R``` (integer value) cars have traversed the bridge from ```left-to-right```.
  - ```number_R2L``` (integer value) cars have traversed the bridge from ```right-to-left```.

The program must accepts ```6``` integers on the command line: ```time_A_L2R```, ```time_A_R2L```, ```time_T_L2R```, ```time_T_R2L```, ```number_L2R```, ```number_R2L```.
