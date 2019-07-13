# Temperatures (Multiple Inputs)

**Input**: 2 different structured files. The first file has the following format:

    sensorId,date,hour,temperature

while the second file:

    date,hour,temperature,sensorId

Example:

    s1,2016-01-01,14:00,20.5
    s2,2016-01-01,14:00,30.2
    s1,2016-01-02,14:10,11.5
    s2,2016-01-02,14:10,30.2

    2016-01-01,14:00,20.1,s3
    2016-01-01,14:00,10.2,s4
    2016-01-02,14:15,31.5,s3
    2016-01-02,14:15,20.2,s4

**Output**: the maximum temperature for each date. Example: 

    2016-01-01	30.2
    2016-01-02	31.5

```sh
BD-Labs/Exercise17/target$ hadoop jar Exercise17-1.jar it.polito.bigdata.hadoop.E17Driver 1 ./in/ ./in2/ ./out/
```
