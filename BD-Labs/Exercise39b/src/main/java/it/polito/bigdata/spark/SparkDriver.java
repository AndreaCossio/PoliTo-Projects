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
        Double threshold = Double.parseDouble(args[2]);
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise39");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

        // Convert the input to a piar rdd
        JavaPairRDD<String, Iterable<String>> grouped = records.mapToPair(x -> {
            String splits[] = x.split(",");
            if (Double.parseDouble(splits[2]) > threshold) {
                return new Tuple2<String, String>(splits[0], splits[1]);
            } else {
                return new Tuple2<String, String>(splits[0], null);
            }
        }).groupByKey();

        JavaPairRDD<String, List<String>> result = grouped.mapValues(x -> {
            ArrayList<String> list = new ArrayList<>();

            for (String s : x) {
                if (s != null) {
                    list.add(s);
                }
            }

            return list;
        });

        // Write the result to the HDFS
        result.saveAsTextFile(outputPath);
        
		// Close the Spark context
		context.close();
	}
}
