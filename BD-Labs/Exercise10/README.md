# Count records (Counters)

**Input**: CSV file. Each line has the following format:

    sensorId,date,value

Example:

    s1,2016-01-01,20.5
    s2,2016-01-01,30.1
    s1,2016-01-02,60.2
    s2,2016-01-02,20.4
    s1,2016-01-03,55.5
    s2,2016-01-03,52.5

**Output**: number of records. Example: 

    6

```sh
BD-Labs/Exercise10/target$ hadoop jar Exercise10-1.jar it.polito.bigdata.hadoop.E10Driver ./in/ ./out/
```
