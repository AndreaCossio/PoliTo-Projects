# PM10 pollution analysis - Maximum value

**Input**: a collection of structured CSV files containing the daily value of PM10 value for a set of sensors with the following format:

    sensorId,date,value

Example:

    s1,2016-01-01,20.5
    s2,2016-01-02,30.1
    s1,2016-01-01,60.2
    s2,2016-01-02,20.4
    s1,2016-01-03,55.5
    s2,2016-01-03,52.5

**Output**: the maximum value. Example: 

    60.2

```sh
BD-Labs/Exercise32/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise32-1.jar ./in/ ./out/
```