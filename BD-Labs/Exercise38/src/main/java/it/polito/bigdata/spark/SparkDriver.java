package it.polito.bigdata.spark;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

import scala.Tuple2;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
        String outputPath = args[1];
        Double threshold = Double.parseDouble(args[2]);
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise38");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

        JavaRDD<String> filteredRecords = records.filter(x -> Double.parseDouble(x.split(",")[2]) > threshold);

        // Convert the input to a piar rdd and reduce counting the number of values
        JavaPairRDD<String, Integer> grouped = filteredRecords.mapToPair(x -> {
            return new Tuple2<String, Integer>(x.split(",")[0], 1);
        }).reduceByKey((a, b) -> a + b);

        // Filter pairs with more than 2 readings
        JavaPairRDD<String, Integer> result = grouped.filter(x -> {
            if (x._2().intValue() >= 2) {
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

