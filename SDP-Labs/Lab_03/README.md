# Instructions

## Exercise 01

Write a C program using Pthreads that tests the ```sem_trywait``` system call on a **Producer & Consumer** problem using two buffers. In particular, the main thread must create a ```Producer``` and a ```Consumer``` thread, and must wait for their termination.

The ```Producer``` thread:

- Loops 10000 times sleeping a random number of milliseconds between ```1``` and ```10```;
- Fills a variable ```ms``` with the current time in milliseconds, using the function:

```c
long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}
```

- Selects randomly a buffer, urgent or normal, in which it will put the value of ```ms```. It must choose the normal buffer ```80%``` of the times;
- Signals on a **semaphore** that something has been produced.

The ```Consumer``` thread:

- Sleeps 10 milliseconds;
- Waits on the semaphore for something to be consumed;
- Tries to get ```ms``` from the urgent buffer, but if it is empty it can proceed to get it from the normal buffer;
- Prints the value of ```ms``` and the buffer identity from which it has got this value.

## Exercise 02

Write a C program using Pthreads that implements the **Producer & Consumer** protocol, where the ```Producer``` thread produces and puts on a shared buffer 10000 integer numbers (from 0 to 9999), and the ```Consumer``` thread gets and prints the received numbers.

You must solve the problem implementing the semaphore functions ```sema_init```, ```sema_wait```, ```sema_post``` using conditions. You cannot use standard semaphores.

## Exercise 03

Write a C program using Pthreads that sorts the content of a binary file including a sequence of integer numbers. Implement a threaded quicksort program where the recursive calls to quicksort are replaced by threads activations, i.e. sorting is done, in parallel, in different regions of the file.

Use ```mmap``` to map the file as a vector in memory.