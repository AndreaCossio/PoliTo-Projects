package it.polito.bigdata.spark;

import java.util.ArrayList;
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
		String inputPath3 = args[2];
        String outputPath = args[3];
        Double threshold = Double.parseDouble(args[4]);
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise44");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);
        JavaRDD<String> preferences = context.textFile(inputPath2);
        JavaRDD<String> movies = context.textFile(inputPath3);

        // Create preferences map
        Map<String, Iterable<String>> preferencesMap = preferences.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[0], splits[1]);
        }).groupByKey().collectAsMap();
        final Broadcast<Map<String, Iterable<String>>> broadcastPreferences = context.broadcast(preferencesMap);

        // Create movies map
        Map<String, String> movieMap = movies.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[0], splits[2]);
        }).collectAsMap();
        final Broadcast<Map<String, String>> broadcastMovies = context.broadcast(movieMap);

        // Create pairs (uID, count(1, preference))
        JavaPairRDD<String, Count> userWatchedGenresCount = records.mapToPair(x -> {
            String splits[] = x.split(",");
            ArrayList<String> genres = new ArrayList<>();
            broadcastPreferences.value().get(splits[0]).iterator().forEachRemaining(genres::add);
            
            if (genres.contains(broadcastMovies.value().get(splits[1]))) {
                return new Tuple2<>(splits[0], new Count(1, 0));
            } else {
                return new Tuple2<>(splits[0], new Count(1, 1));
            }
        });

        JavaRDD<String> result = userWatchedGenresCount
            .reduceByKey((a, b) -> {
                return new Count(a.tot + b.tot, a.critical + b.critical);
            })
            .mapValues(x -> (double) x.critical / x.tot)
            .filter(x -> x._2() > threshold)
            .keys();
        
        // Write the result to the HDFS
        result.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}
