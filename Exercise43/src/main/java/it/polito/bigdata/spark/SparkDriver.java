package it.polito.bigdata.spark;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.broadcast.Broadcast;

import scala.Tuple2;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
		String inputPath2 = args[1];
        String outputPath = args[2];
        String outputPath2 = args[3];
        String outputPath3 = args[4];
        Integer n = Integer.parseInt(args[5]);
        Double threshold = Double.parseDouble(args[6]);
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise43");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input files
        JavaRDD<String> readings = context.textFile(inputPath).cache();
        JavaRDD<String> neighbors = context.textFile(inputPath2);

        /**
         * PART 1 - Select stations with high critical percentage
         */
        // Create pairs (sID, count(tot, critical))
        JavaPairRDD<String, Count> countPairs = readings.mapToPair(x -> {
            String splits[] = x.split(",");
            if (Integer.parseInt(splits[5]) < n) {
                return new Tuple2<>(splits[0], new Count(1, 1));
            } else {
                return new Tuple2<>(splits[0], new Count(1, 0));
            }
        });

        // Create pairs (sID, percentage)
        JavaPairRDD<String, Double> percentagePairs = countPairs
            .reduceByKey((a, b) -> {
                return new Count(a.tot + b.tot, a.critical + b.critical);
            })
            .mapValues(x -> (double) x.critical / x.tot)
            .filter(x -> x._2() > threshold);
        
        // Create pairs (percentage, sID)
        JavaPairRDD<Double, String> result1 = percentagePairs.mapToPair(x -> {
            return new Tuple2<>(x._2(), x._1());
        }).sortByKey(false);

        // Write the result to the HDFS
        result1.saveAsTextFile(outputPath);

        /**
         * PART 2 - Compute critical percentage per timeslot
         */
        // Create pairs (timeslot_sID, Count(tot, critical))
        JavaPairRDD<String, Count> countTimePairs = readings.mapToPair(x -> {
            
            String splits[] = x.split(",");

            int minTime = 4 * (Integer.parseInt(splits[2]) / 4);
            String key = "ts[" + minTime + "-" + (minTime + 3) + "]_" + splits[0];

            if (Integer.parseInt(splits[5]) < n) {
                return new Tuple2<>(key, new Count(1, 1));
            } else {
                return new Tuple2<>(key, new Count(1, 0));
            }
        });

        // Create pairs (timeslot_sID, percentage)
        JavaPairRDD<String, Double> percentageTimePairs = countTimePairs
            .reduceByKey((a, b) -> {
                return new Count(a.tot + b.tot, a.critical + b.critical);
            })
            .mapValues(x -> (double) x.critical / x.tot)
            .filter(x -> x._2() > threshold);
        
        // Create pairs (percentage, sID)
        JavaPairRDD<Double, String> result2 = percentageTimePairs.mapToPair(x -> {
            return new Tuple2<>(x._2(), x._1());
        }).sortByKey(false);

        // Write the result to the HDFS
        result2.saveAsTextFile(outputPath2);

        /**
         * PART 3 - Full stations
         */
        // Create neighbor mapping and broadcast it
        Map<String, ArrayList<String>> neighborMap = neighbors.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[0], new ArrayList<>(Arrays.asList(splits[1].split(" "))));
        }).collectAsMap();
        final Broadcast<Map<String, ArrayList<String>>> neighborsBroadcast = context.broadcast(neighborMap);
        
        // Select full stations
        JavaRDD<String> fullStations = readings.filter(x -> Integer.parseInt(x.split(",")[5]) == 0);

        // Full stations' readings for each timestamp
        JavaPairRDD<String, Iterable<String>> fullTimeStations = fullStations.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[1] + splits[2] + splits[3], x);
        }).groupByKey();

        // Each pair contains a list of full stations at the given timestamp.
        // Check if all the neighbors are also full.
        JavaRDD<String> result3 = fullTimeStations.flatMap(x -> {

            ArrayList<String> stations = new ArrayList<>();
            ArrayList<String> filtered = new ArrayList<>();

            for (String line : x._2()) {
                stations.add(line.split(",")[0]);
            }

            for (String line : x._2()) {
                boolean allFull = true;
                for (String ne : neighborsBroadcast.value().get(line.split(",")[0])) {
                    if (!stations.contains(ne)) {
                        allFull = false;
                    }
                }
                if (allFull) {
                    filtered.add(line);
                }
            }

            return filtered.iterator();
        });

        // Write the result to the HDFS
        result3.saveAsTextFile(outputPath3);

		// Close the Spark context
		context.close();
	}
}
