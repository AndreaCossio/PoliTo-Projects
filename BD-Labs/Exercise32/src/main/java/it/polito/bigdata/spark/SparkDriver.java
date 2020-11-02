package it.polito.bigdata.spark;

import java.util.List;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise32");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

		// Map the records to the value and find the highest one
        List<Double> result = records.map(x -> Double.parseDouble(x.split(",")[2])).top(1);
        
        // Print to stdout
        for (Double d : result) {
            System.out.println(d);
        }

		// Close the Spark context
		context.close();
	}
}
