# Income analysis (double job)

**Input**: text file. Each line has the following format:

    date    income

Example:

    2015-11-01	1000
    2015-11-02	1305
    2015-12-01	500
    2015-12-02	750
    2016-01-01	345
    2016-01-02	1145
    2016-02-03	200
    2016-02-04	500

**Output**: total income of the month and average monthly income of the year. Example: 

    2015-11	2305.0
    2015-12	1250.0
    2016-01	1490.0
    2016-02	700.0

    2015	1777.5
    2016	1095.0

```sh
BD-Labs/Exercise08/target$ hadoop jar Exercise08-1.jar it.polito.bigdata.hadoop.E08Driver 1 ./in/ ./out/ ./out2/
```