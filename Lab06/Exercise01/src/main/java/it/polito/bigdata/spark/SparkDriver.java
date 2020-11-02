package it.polito.bigdata.spark;

import java.util.ArrayList;
import java.util.List;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

import scala.Tuple2;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
        String outputPath = args[1];
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Lab06_Ex1");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

        // Create pairs (userId, list of product Id)
        JavaPairRDD<String, Iterable<String>> userProducts = records.filter(x -> !x.startsWith("Id,"))
            .mapToPair(x -> {
                String splits[] = x.split(",");
                return new Tuple2<>(splits[2], splits[1]);
            }).groupByKey();

        // Create pairs (p1-p2, 1)
        JavaPairRDD<String, Integer> pairList = userProducts.values().flatMapToPair(x -> {
            List<Tuple2<String, Integer>> products = new ArrayList<>();

            for (String p1 : x) {
                for (String p2 : x) {
                    Tuple2<String, Integer> t = new Tuple2<>(p1 + "-" + p2, 1);
                    if (p1.compareTo(p2) > 0 && !products.contains(t)) {
                        products.add(t);
                    } 
                }
            }

            return products.iterator();
        });

        // Create pairs (p1-p2, n)
        JavaPairRDD<String, Integer> pairCount = pairList.reduceByKey((a, b) -> a + b).filter(x -> x._2() > 1);

        // Reverse pairs
        JavaPairRDD<Integer, String> result = pairCount.mapToPair(x -> new Tuple2<>(x._2(), x._1())).sortByKey(false);

        // Print top 10
        for (Tuple2<String, Integer> t : pairCount.top(10, new FreqComparator())) {
            System.out.println(t._1() + "," + t._2());
        }

        // Write the result to the HDFS
        result.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}
