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

**Output**: min and max values for each sensor. Example: 

    s1	max=60.2_min=20.5
    s2	max=52.5_min=20.4

```sh
BD-Labs/Exercise06/target$ hadoop jar Exercise06-1.jar it.polito.bigdata.hadoop.E06Driver 1 ./in/ ./out/
```