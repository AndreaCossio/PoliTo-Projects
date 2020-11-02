# Instructions

## Exercise 01

The following program copies a file, whose name is received as first parameter on the command line, into another file, whose name is passed as the second parameter on the command line.

```cpp
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define BUF_SIZE 100

INT _tmain (INT argc, LPTSTR argv [])
{
  HANDLE Hon, hOut;
  DWORD nIn, nOut;
  CHAR buffer [BUF_SIZE];

  if (argc != 3) {
    fprintf (stderr, "Usage: cp file1 file2\n");
    return 1;
  }

  hIn = CreateFile (argv[1], GENERIC_READ, FILE_SHARE_READ, NULL,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hIn == INVALID_HANDLE_VALUE) {
    fprintf ("Cannot open input file. Error: %x\n", GetLastError ());
    return 2;
  }

  hOut = CreateFile (argv[2], GENERIC_WRITE, 0, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hOut == INVALID_HANDLE_VALUE) {
    fprintf (stderr, "Cannot open output file. Error: %x\n", GetLastError ());
    CloseHandle(hIn);
    return 3;
  }

  while (ReadFile (hIn, buffer, BUF_SIZE, &nIn, NULL) && nIn > 0) {
    WriteFile (hOut, buffer, BUF_SIZE, &nOut, NULL);
    if (nIn != nOut) {
      fprintf (stderr, "Fatal write error: %x\n", GetLastError ());
      CloseHandle(hIn); CloseHandle(hOut);
      return 4;
    }
  }

  CloseHandle (hin);
  CloseHandle (hOut);
  return 0;
}
```

Perform the following steps:

- Cut-and-paste the program into a ```.cpp``` file
- Create a new project and insert the program within the MS Visual Studio environment.
- Set the command line parameters.
- Build (compile) the project.
- As it contains (3?) syntax errors correct them.
- Once obtained the executable code, run it.
- As it contains a bug, debug it using the basic features of the Visual Studio debugger.

## Exercise 02

An ASCII file has lines with the following format:

- An identifier
- A register number (max. 6 digits)
- A surname (max 30 chars)
- A name (max 30 chars)
- An examination mark

The following is a correct example of such a file:

    1 100000 Romano Antonio 25
    2 150000 Fabrizi Aldo 22
    3 200000 Verdi Giacomo 15
    4 250000 Rossi Luigi 30

Write a C program in the MS Visual Studio environment which is able to:

- Read such a file and create a new file with the exact same content but stored in **binary** format and with *fixed-length* records (integer + long integer + 30 characters + 30 characters + integer).
- Verify the previous step (i.e., to be sure the binary file has
  been stored in a proper way) reading the binary file and writing its content to standard output.

Input and output filenames are passed from the command line.

## Exercise 03

A database is given on a single **binary** file with *fixed-length* records. The format of the file is the one generated in [Exercise 02](#exercise-02).

The program has to implement a user menu with the following options:

- ```R n```: read from the database all data fields for the student number ```n``` and print them to standard output
- ```W n```: read from standard input all data fields for the student number ```n``` and write them on the file in position ```n```
- ```E```: end the program

The input filename is passed from the command line.

### Version A

Read the file using SetFilePointerEx.

### Version B

Read the file using an overlapped data structure.

### Version C

Lock each record before reading (or writing) it, and release the same record as soon as the operation has been performed. (Notice that locking for now is useless, but it will be useful with multi-threaded applications).
