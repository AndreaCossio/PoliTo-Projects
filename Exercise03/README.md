# PM10 pollution analysis

**Input**: structured textual file. Each line has the following format:

    sensorId,date   value

Example:

    s1,2016-01-01	20.5
    s2,2016-01-01	30.1
    s1,2016-01-02	60.2
    s2,2016-01-02	20.4
    s1,2016-01-03	55.5
    s2,2016-01-03	52.5

**Output**: number of occurrences of each word appearing in each input file. Example: 

    s1  2
    s2  1

```sh
BD-Labs/Exercise02/target$ hadoop jar Exercise02-1.jar it.polito.bigdata.hadoop.E02Driver 1 ./in/ ./out/
```