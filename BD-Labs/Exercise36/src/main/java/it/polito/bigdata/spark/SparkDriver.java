package it.polito.bigdata.spark;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
	
public class SparkDriver {
	
	public static void main(String[] args) {

		String inputPath = args[0];
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise36");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> records = context.textFile(inputPath);

        // Gather the sum of the values
        Double sum = records.map(
            x -> new Double(x.split(",")[2])
        ).reduce((Double a, Double b) -> new Double(a + b));

        // Calc average
        System.out.println(sum / records.count());

		// Close the Spark context
		context.close();
	}
}

