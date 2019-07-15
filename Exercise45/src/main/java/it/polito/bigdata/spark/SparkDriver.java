package it.polito.bigdata.spark;

import java.util.ArrayList;
import java.util.HashMap;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

import scala.Tuple2;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
		String inputPath2 = args[1];
		String inputPath3 = args[2];
        String outputPath = args[3];
        Double threshold = Double.parseDouble(args[4]);
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise45");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);
        JavaRDD<String> preferences = context.textFile(inputPath2);
        JavaRDD<String> movies = context.textFile(inputPath3);

        // Create pairs (movie, user)
        JavaPairRDD<String, String> movieUser = records.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[1], splits[0]);
        });

        // Create pairs (movie, genre)
        JavaPairRDD<String, String> movieGenre = movies.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[0], splits[2]);
        });

        //Create pairs (user, genre)
        JavaPairRDD<String, String> userPreference = preferences.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[0], splits[1]);
        });

        // Join movies genres and user
        JavaPairRDD<String, Tuple2<String, String>> movieUserGenre = movieUser.join(movieGenre);

        // Create pairs (user, genreWatched)
        JavaPairRDD<String, String> userGenresWatched = movieUserGenre.mapToPair(x -> {
            return new Tuple2<>(x._2()._1(), x._2()._2());
        });

        // Cogroup the list of watched genre and the preferenced ones
        // Pairs: (userId, (genre watched, genre preferences))
        JavaPairRDD<String, Tuple2<Iterable<String>, Iterable<String>>> table = userGenresWatched.cogroup(userPreference);
        
        // Filter and keep only those lines associated to a misleading user
        JavaPairRDD<String, Tuple2<Iterable<String>, Iterable<String>>> filteredTable = table.filter(x -> {

            int tot = 0, misleading = 0;
            ArrayList<String> pref = new ArrayList<>();
            x._2()._2().iterator().forEachRemaining(pref::add);

            for (String genre : x._2()._1()) {
                tot++;
                if (!pref.contains(genre)) {
                    misleading++;
                }
            }

            return (double) misleading / tot > threshold;
        });

        // Filter and select misleading genres seen at least 5 times
        JavaPairRDD<String, String> result = filteredTable.flatMapValues(x -> {

            ArrayList<String> misleadingGenres = new ArrayList<>();

            ArrayList<String> pref = new ArrayList<>();
            x._2().iterator().forEachRemaining(pref::add);

            HashMap<String, Integer> countGenres = new HashMap<>();

            for (String genre : x._1()) {
                if (!pref.contains(genre)) {
                    Integer count = countGenres.get(genre);
                    if (count == null) {
                        countGenres.put(genre, new Integer(1));
                    } else {
                        countGenres.put(genre, count + 1);
                    }
                }
            }

            for (String genre : countGenres.keySet()) {
                if (countGenres.get(genre) >= 5) {
                    misleadingGenres.add(genre);
                }
            }

            return misleadingGenres;
        });
        
        // Write the result to the HDFS
        result.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}
