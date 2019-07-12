# PM10 pollution analysis - City zone

**Input**: structured textual file. Each line has the following format:

    zoneId,date   value

Example:

    zone1,2016-01-01	20.5
    zone2,2016-01-01	30.1
    zone1,2016-01-02	60.2
    zone2,2016-01-02	20.4
    zone1,2016-01-03	55.5
    zone2,2016-01-03	52.5

**Output**: list of dates in which the value is above the threshold for each city zone. Example: 

    zone1	[2016-01-03, 2016-01-02]
    zone2	[2016-01-03]

```sh
BD-Labs/Exercise04/target$ hadoop jar Exercise04-1.jar it.polito.bigdata.hadoop.E04Driver 1 ./in/ ./out/ 50
```