# PM10 pollution analysis - Average

**Input**: CSV file. Each line has the following format:

    sensorId,date,value

Example:

    s1,2016-01-01,20.5
    s2,2016-01-01,30.1
    s1,2016-01-02,60.2
    s2,2016-01-02,20.4
    s1,2016-01-03,55.5
    s2,2016-01-03,52.5

**Output**: average value for each sensor. Example: 

    s1	45.4
    s2	34.3

```sh
BD-Labs/Exercise05/target$ hadoop jar Exercise05-1.jar it.polito.bigdata.hadoop.E05Driver 1 ./in/ ./out/
```