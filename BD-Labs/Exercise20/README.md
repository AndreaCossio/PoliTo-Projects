# Temperatures (Multiple Outputs)

**Input**: CSV file. Each line has the following format:

    sensorId,date,hour,temperature

Example:

    s1,2016-01-01,14:00,20.5
    s2,2016-01-01,14:00,30.2
    s1,2016-01-02,14:10,11.5
    s2,2016-01-02,14:10,30.2

**Output**: 2 sets of files containing records with temperatures below and above 30.0. Example: 

    s2,2016-01-01,14:00,30.2
    s2,2016-01-02,14:10,30.2

    s1,2016-01-01,14:00,20.5
    s1,2016-01-02,14:10,11.5

```sh
BD-Labs/Exercise20/target$ hadoop jar Exercise20-1.jar it.polito.bigdata.hadoop.E20Driver ./in/ ./out/
```
