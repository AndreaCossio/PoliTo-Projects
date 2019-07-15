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
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise37");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

        // Convert the input to a piar rdd keeping the maximum values for each sensor
        JavaPairRDD<String, Double> result = records.mapToPair(x -> {
            return new Tuple2<String, Double>(x.split(",")[0], new Double(x.split(",")[2]));
        }).reduceByKey((a,b) -> {
            if (a > b) {
                return a;
            } else {
                return b;
            }
        });

        // Write the result to HDFS
        result.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}

