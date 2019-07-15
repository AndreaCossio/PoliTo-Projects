# Critical bike sharing analysis

**Input**: a CSV file containing sensor readings with the following format:

    stationId,date,hour,minute,#bike,#freeslots

The readings are taken from each sensor every 5 minutes; some readings may be lost.

Example:

    s1,2015-05-01,00,00,5,4
    s2,2015-05-01,00,00,4,4
    s3,2015-05-01,00,00,6,3
    s4,2015-05-01,00,00,5,2
    s5,2015-05-01,00,00,1,6
    s1,2015-05-01,00,05,4,5
    s2,2015-05-01,00,05,5,3
    s3,2015-05-01,00,05,7,2
    s4,2015-05-01,00,05,6,1
    s5,2015-05-01,00,05,0,7
    s1,2015-05-01,15,00,3,6
    s3,2015-05-01,15,00,4,5
    s4,2015-05-01,15,00,6,1
    s5,2015-05-01,15,00,5,2
    s1,2015-05-01,15,05,4,5
    s2,2015-05-01,15,05,5,3
    s3,2015-05-01,15,05,5,4
    s5,2015-05-01,15,05,3,4
    s5,2015-05-02,00,00,3,4
    s1,2015-05-02,00,00,9,0
    s2,2015-05-02,00,00,8,0
    s3,2015-05-02,00,00,9,0

A CSV file containing the list of neighbors of each station with the following format:

    stationId,[list of neighbors]

Example:

    s1,s2 s3
    s2,s1 s5
    s3,s1
    s4,s5
    s5,s4 s2

**Output**: the percentage of critical situations (less than a given number of free slots). Keep only percentages that are higher than a given threshold. Example (n = 3, thr = 0.8):

    (1.0,s4)

The percentage of critical situations per time slot:

    (1.0,ts[12-15]_s4)
    (1.0,ts[0-3]_s4)

The list of full stations that have only full neighbors:

    s1,2015-05-02,00,00,9,0
    s3,2015-05-02,00,00,9,0

```sh
BD-Labs/Exercise43/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise43-1.jar ./in/readings.txt ./in/neighbors.txt ./out/ ./out1/ ./out2/ 3 0.8
```
