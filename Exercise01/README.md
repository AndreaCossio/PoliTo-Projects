# Word Count

**Input**: textual file. Example:

    Toy example
    file for Hadoop.
    Hadoop running
    example.

**Output**: number of occurrences of each word appearing in the input file. Example: 

    toy	1
    example	2
    file	1
    for	1
    hadoop	2
    running	1

```sh
BD-Labs/Exercise01/target$ hadoop jar Exercise01-1.jar it.polito.bigdata.hadoop.E01Driver 1 ./in/input.txt ./out/
```