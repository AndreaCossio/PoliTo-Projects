# Income analysis (Top 1)

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

**Output**: top income. Example: 

    2015-11-02	1305

```sh
BD-Labs/Exercise13/target$ hadoop jar Exercise13-1.jar it.polito.bigdata.hadoop.E13Driver 1 ./in/ ./out/
```
