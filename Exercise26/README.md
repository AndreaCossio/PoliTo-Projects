# Dictionary (Cached files)

**Input**: textual file with one sentence per line. Example:

    TEST CONVERTION WORD TO INTEGER
    SECOND LINE TEST WORD TO INTEGER

A dictionary containing the translation of words to numbers. Example:

    1	CONVERTION
    2	INTEGER
    3	LINE
    4	SECOND
    5	TEST
    6	TO
    7	WORD

**Output**: the document translated. Example:

    5 1 7 6 2
    4 3 5 7 6 2

```sh
BD-Labs/Exercise26/target$ hadoop jar Exercise26-1.jar it.polito.bigdata.hadoop.Driver ./in/ ./out/ ../dictionary.txt
```
