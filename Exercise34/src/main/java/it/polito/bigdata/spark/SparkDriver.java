package it.polito.bigdata.spark;

import java.util.List;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
        String outputPath = args[1];
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise34");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath).cache();

		// Map the records to the value and find the highest one
        List<Double> list = records.map(x -> Double.parseDouble(x.split(",")[2])).top(1);

        // Filter the input records with the highest value
        JavaRDD<String> result = records.filter(x -> list.get(0).equals(Double.parseDouble(x.split(",")[2])));
        
        // Store the result in the output folder
		result.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}
