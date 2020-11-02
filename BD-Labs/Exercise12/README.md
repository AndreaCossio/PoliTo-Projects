# PM10 pollution analysis - Threshold (Map-only)

**Input**: structured textual file. Each line has the following format:

    sensorId,date   value

Example:

    s1,2016-01-01	20.5
    s2,2016-01-01	30.1
    s1,2016-01-02	60.2
    s2,2016-01-02	20.4
    s1,2016-01-03	55.5
    s2,2016-01-03	52.5

**Output**: records with a value below the threshold. Example (thr = 21):

    s1,2016-01-01	20.5
    s2,2016-01-02	20.4

```sh
BD-Labs/Exercise12/target$ hadoop jar Exercise12-1.jar it.polito.bigdata.hadoop.E12Driver 1 ./in/ ./out/ 50
```
