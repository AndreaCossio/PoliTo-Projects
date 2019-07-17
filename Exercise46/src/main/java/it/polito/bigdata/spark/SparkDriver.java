package it.polito.bigdata.spark;

import java.util.ArrayList;
import java.util.HashMap;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

import scala.Tuple2;

public class SparkDriver {

    public static void main(String[] args) {

        String inputPath = args[0];
        String outputPath = args[1];

        // Setup
        SparkConf conf = new SparkConf().setAppName("Exercise46");
        JavaSparkContext context = new JavaSparkContext(conf);

        // Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

        // Create pairs (timestamp, list of temperatures)
        JavaRDD<Iterable<TimestampTemperature>> windows = records.flatMapToPair(x -> {

            int i;
            String splits[] = x.split(",");
            ArrayList<Tuple2<Integer, TimestampTemperature>> pairs = new ArrayList<>();

            for (i = 0; i < 3; i++) {
                pairs.add(new Tuple2<>(Integer.parseInt(splits[0]) - 60 * i,
                        new TimestampTemperature(Integer.parseInt(splits[0]), Double.parseDouble(splits[1]))));
            }

            return pairs.iterator();
        }).groupByKey().values();

        // Filter windows
        JavaRDD<Iterable<TimestampTemperature>> result = windows.filter(x -> {

            int min = Integer.MIN_VALUE;
            HashMap<Integer, Double> map = new HashMap<>();

            // The 3 timestamps and temperatures associated with the current window
            x.iterator().forEachRemaining(ts -> map.put(ts.getTimestamp(), ts.getTemperature()));

            if (map.size() == 3) {
                min = map.keySet().stream().sorted().findFirst().get();

                for (int i = 0; i < 2; i++) {
                    if (map.get(min) >= map.get(min + 60)) {
                        return false;
                    }
                    min += 60;
                }

                return true;
            } else {
                return false;
            }
        });

        // Write the result to the HDFS
        result.saveAsTextFile(outputPath);

        // Close the Spark context
        context.close();
    }
}
