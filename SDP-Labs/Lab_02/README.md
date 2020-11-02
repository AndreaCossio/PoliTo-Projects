# Instructions

## Exercise 01

Implement a concurrent program in C language, using Pthreads, which creates two client threads, then it acts as a server.

The ```client``` thread:

- Reads numbers from its binary file, ```fv1.b``` or ```fv2.b```;
- Stores the number in a global variable ```g```;
- Performs a signals on a ```semaphore``` to indicate to the server that the variable is ready;
- Waits on another ```semaphore``` the server;
- Prints the result and its identifier.

The ```server``` thread:

- Waits the signal of the clients;
- Multiplies ```g``` by ```3```;
- Signals the client that the string is ready to be printed.

The ```main``` thread waits the end of the other threads, and prints the total number of served requests.

## Exercise 02

Implement a concurrent program in C language, using Pthreads, which generates two threads, and then waits for their termination.

The first thread ```th1``` must:

- Sleep a random number of milliseconds ```t``` in range ```[1 5]```;
- Print ```waiting on semaphore after t milliseconds```
- Wait on a semaphore ```s```, initialized to 0, no more than ```tmax``` milliseconds (```tmax``` is passed as an argument of the command line);
- Print ```wait returned normally``` if a ```sem_post(s)``` was performed by the second thread ```th2``` within ```tmax``` milliseconds from the wait call (or if the ```sem_post``` call is performed by ```th2``` before the ```sem_wait``` call performed by ```th1```), otherwise it must print ```wait on semaphore s returned for timeout```.

The second thread ```th2``` must:

- Sleep a random number of milliseconds ```t``` in range ```[1000 10000]```;
- Print ```performing signal on semaphore s after t milliseconds```.

For the first thread, you must implement and use a function with prototype ```int wait_with_timeout (sem_t *S, int tmax)```, which, using the appropriate system calls for the management of semaphores and ```SIGALARM``` signals, allows you to define the maximum time that a process can be blocked on the semaphore ```s``` queue before it is unblocked, and can proceed regardless of a call to ```sem_post(s)```.

## Exercise 03

Implement [Exercise 02](#exercise-02) using ```sem_timedwait```.