# Log Filtering + Map and distinct

**Input**: a simplified log of a web browser. Example:

    66.249.69.97 - - [24/Sep/2014:22:25:44 +0000] "GET http://www.google.com/bot.html”
    66.249.69.97 - - [24/Sep/2014:22:26:44 +0000] "GET http://www.google.com/how.html”
    66.249.69.97 - - [24/Sep/2014:22:28:44 +0000] "GET http://dbdmg.polito.it/course.html”
    71.19.157.179 - - [24/Sep/2014:22:30:12 +0000] "GET http://www.google.com/faq.html”
    66.249.69.97 - - [24/Sep/2014:31:28:44 +0000] "GET http://dbdmg.polito.it/thesis.html”
    66.249.69.97 - - [24/Sep/2014:22:26:44 +0000] "GET http://www.google.com/how.html”
    56.249.69.97 - - [24/Sep/2014:22:26:44 +0000] "GET http://www.google.com/how.html”

**Output**: the list of distinct IP addresses associated with the connections to a given domain. Example (domain = "google.com"): 

    66.249.69.97
    71.19.157.179
    56.249.69.97

```sh
BD-Labs/Exercise31/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise31-1.jar ./in/ ./out/ google.com
```