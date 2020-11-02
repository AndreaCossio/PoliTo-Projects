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
		SparkConf conf = new SparkConf().setAppName("Exercise35");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath).cache();

        // Gather the maximum value
        List<Double> list = records.map(
            x -> Double.parseDouble(x.split(",")[2])
        ).top(1);

        // Filter records and map to distinct dates
        JavaRDD<String> result = records
            .filter(x -> list.get(0).equals(Double.parseDouble(x.split(",")[2])))
            .map(x -> x.split(",")[1])
            .distinct();
        
        // Write the result to HDFS
        result.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}

