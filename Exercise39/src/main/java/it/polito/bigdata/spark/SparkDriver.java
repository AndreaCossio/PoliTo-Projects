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
		SparkConf conf = new SparkConf().setAppName("Exercise39");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

        JavaRDD<String> filteredRecords = records.filter(x -> Double.parseDouble(x.split(",")[2]) > threshold);

        // Convert the input to a piar rdd
        JavaPairRDD<String, Iterable<String>> grouped = filteredRecords.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<String, String>(splits[0], splits[1]);
        }).groupByKey();

        // Write the result to the HDFS
        grouped.saveAsTextFile(outputPath);
        
		// Close the Spark context
		context.close();
	}
}
