# PM10 pollution analysis - Maximum value records

**Input**: a collection of structured CSV files containing the daily value of PM10 value for a set of sensors with the following format:

    sensorId,date,value

Example:

    s1,2016-01-01,20.5
    s2,2016-01-01,30.1
    s1,2016-01-02,60.2
    s2,2016-01-02,20.4
    s1,2016-01-03,60.2
    s2,2016-01-03,52.5

**Output**: the records associated with the maximum value. Example: 

    s1,2016-01-02,60.2
    s1,2016-01-03,60.2

```sh
BD-Labs/Exercise34/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise34-1.jar ./in/ ./out/
```