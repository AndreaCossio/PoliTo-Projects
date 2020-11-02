# PM10 pollution analysis - Critical date analysis

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

**Output**: list of dates with a PM10 value greater than a given threshold for each sensor. Example (thr = 50): 

    (s1,[2016-01-02, 2016-01-03])
    (s2,[2016-01-03])
    (s3,[2016-01-03])

```sh
BD-Labs/Exercise39/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise39-1.jar ./in/ ./out/ 50
```