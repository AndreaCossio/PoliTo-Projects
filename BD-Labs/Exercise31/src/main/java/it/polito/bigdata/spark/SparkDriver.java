package it.polito.bigdata.spark;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
		String outputPath = args[1];
		String word = args[2];
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise31");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
		JavaRDD<String> list = context.textFile(inputPath);

		// Filter lines containing the given word and map them to the IP
		JavaRDD<String> result = list.filter(x -> x.toLowerCase().contains(word)).map(x -> x.split(" -")[0]).distinct();
		
		// Store the result in the output folder
		result.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}
