# Inverted index

**Input**: text file with a set of sentences. Each line has the following format:

    sentenceId    sentence

Example:

    Sentence#1	Hadoop or Spark
    Sentence#2	Hadoop or Spark and Java
    Sentence#3	Hadoop and Big Data

**Output**: list of sentence Ids for each word. Example: 

    and	[Sentence#3, Sentence#2]
    big	[Sentence#3]
    data	[Sentence#3]
    hadoop	[Sentence#3, Sentence#2, Sentence#1]
    java	[Sentence#2]
    or	[Sentence#2, Sentence#1]
    spark	[Sentence#2, Sentence#1]

```sh
BD-Labs/Exercise07/target$ hadoop jar Exercise07-1.jar it.polito.bigdata.hadoop.E07Driver 1 ./in/ ./out/
```