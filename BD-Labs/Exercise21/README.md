# Stopwords (Cached files)

**Input**: textual file with one sentence per line. Example:

    This is the first sentence and it contains some stopwords
    Second sentence with a stopword here and another here
    Third sentence of the stopword example

A textual file containing stopwords. Example:

    a
    an
    and
    or
    i
    you
    we
    he
    she
    they
    it
    of
    the

**Output**: all the original sentences without stopwords. Example:

    This is first sentence contains some stopwords 
    Second sentence with stopword here another here 
    Third sentence stopword example 

```sh
BD-Labs/Exercise21/target$ hadoop jar Exercise21-1.jar it.polito.bigdata.hadoop.E21Driver ./in/ ./out/ ../stopwords.txt
```
