# Timeseries analysis

**Input**: a CSV file containing list of temperature readings (sample rate 1 minute) with the following format:

    timestamp,temperature

Example:

    1451606400,12.1
    1451606460,12.2
    1451606520,13.5
    1451606580,14.0
    1451606640,14.0
    1451606700,15.5
    1451606760,15.0

**Output**: all windows of 3 consecutive readings characterized by an increasing trend. Example:

    [1451606460,12.2, 1451606520,13.5, 1451606580,14.0]
    [1451606400,12.1, 1451606460,12.2, 1451606520,13.5]

```sh
BD-Labs/Exercise46/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise46-1.jar ./in/ ./out/
```
