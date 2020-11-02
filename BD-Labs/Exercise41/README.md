# PM10 pollution analysis - Order and TopK

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
    s4,2016-01-04,12.2

**Output**: list of the TopK sensors ordered by number of critical dates. Example (thr = 50, k = 2): 

    (2,s1)
    (1,s2)

```sh
BD-Labs/Exercise41/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise41-1.jar ./in/ ./out/ 50 2
```