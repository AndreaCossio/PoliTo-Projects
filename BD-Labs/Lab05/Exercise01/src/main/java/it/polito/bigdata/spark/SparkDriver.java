package it.polito.bigdata.spark;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
        String outputPath = args[1];
        String prefix = args[2];

        Integer maxfreq;
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Lab05_Ex1");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

        // Keep only those lines that start with the given prefix
        JavaRDD<String> result1 = records.filter(x -> x.startsWith(prefix));

        // Find maxfreq
        maxfreq = result1.map(x -> Integer.parseInt(x.split("\\t")[1])).top(1).get(0);

        // Print info
        System.out.println("Selected lines: " + result1.count());
        System.out.println("Maxfreq: " + maxfreq);

        // Filter frequent words (80% of maxfreq)
        JavaRDD<String> result2 = result1.filter(x -> Double.parseDouble(x.split("\\t")[1]) > (double) maxfreq * 0.8);

        // Print info
        System.out.println("Frequent lines: " + result2.count());

        // Write the result to the HDFS
        result2.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}
