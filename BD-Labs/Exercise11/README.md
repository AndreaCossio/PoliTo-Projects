# PM10 pollution analysis - Average (Custom Data Type)

**Input**: CSV files. Each line has the following format:

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
BD-Labs/Exercise11/target$ hadoop jar Exercise11-1.jar it.polito.bigdata.hadoop.E11Driver 1 ./in/ ./out/
```
