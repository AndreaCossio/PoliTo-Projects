# Word Count - in-Mapper combiner

**Input**: HDFS folder. Example:

    Test of the word count program
    The word program is the Hadoop hello word program
    Example document for hadoop word count

    Test of the word count program

    The word program is the Hadoop hello word program
    Example document for hadoop word count

**Output**: number of occurrences of each word appearing in each input file. Example: 

    count	4
    document	2
    example	2
    for	2
    hadoop	4
    hello	2
    is	2
    of	2
    program	6
    test	2
    the	6
    word	8

```sh
BD-Labs/Exercise09/target$ hadoop jar Exercise09-1.jar it.polito.bigdata.hadoop.E09Driver 1 ./in/ ./out/
```
