# PM10 pollution analysis - Threshold

**Input**: a collection of structured CSV files containing the daily value of PM10 value for a set of sensors with the following format:

    sensorId,date,value

Example:

    s1,2016-01-01,20.5
    s2,2016-01-01,30.1
    s1,2016-01-02,60.2
    s2,2016-01-02,20.4
    s1,2016-01-03,60.2
    s2,2016-01-03,52.5
    s3,2016-01-03,60.2

**Output**: the sensors with more than 2 readings with a value of PM10 greater than a given threshold. Example (thr = 50): 

    s1,2

```sh
BD-Labs/Exercise38/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise38-1.jar ./in/ ./out/ 50
```