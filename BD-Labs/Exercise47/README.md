# Full station identification in real-time

**Input**: a CSV file containing a list of bike statons with the following format:

    id  longitude   latitude    name

Example:

    id	longitude	latitude	name
    1	2.180019	41.397978	Gran Via Corts Catalanes
    2	2.176414	41.394381	Plaza Tetuan
    3	2.181164	41.393750	Ali Bei
    4	2.181400	41.393364	Ribes

A streaming of readings about the status of the stations with the following format:

    stationId,#freeSlots,#usedSlots,timestamp

**Output**: the status of full stations for each reading. Example:

    -------------------------------------------
    Time: 1563209950000 ms
    -------------------------------------------
    (23344454,Plaza Tetuan)
    (04543534,Gran Via Corts Catalanes)

    -------------------------------------------
    Time: 1563209952000 ms
    -------------------------------------------

    -------------------------------------------
    Time: 1563209954000 ms
    -------------------------------------------

```sh
BD-Labs/Exercise47/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local[10] Exercise47-1.jar ./in/stations.csv out
```
