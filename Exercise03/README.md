# PM10 pollution analysis - Threshold

**Input**: structured textual file. Each line has the following format:

    sensorId,date   value

Example:

    s1,2016-01-01	20.5
    s2,2016-01-01	30.1
    s1,2016-01-02	60.2
    s2,2016-01-02	20.4
    s1,2016-01-03	55.5
    s2,2016-01-03	52.5

**Output**: number of values above the threshold for each sensor. Example (thr = 50):

    s1  2
    s2  1

```sh
BD-Labs/Exercise03/target$ hadoop jar Exercise03-1.jar it.polito.bigdata.hadoop.E03Driver 1 ./in/ ./out/ 50
```
