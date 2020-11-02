package it.polito.bigdata.spark;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaSparkContext;

import scala.Tuple2;
	
public class SparkDriver {
	
	public static void main(String[] args) {

        String inputPath = args[0];
        String inputPath2 = args[1];
        String outputPath = args[2];
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise42");
		JavaSparkContext context = new JavaSparkContext(conf);

		// Read the content of the input file
        JavaRDD<String> questions = context.textFile(inputPath);
        JavaRDD<String> answers = context.textFile(inputPath2);

        // Create PairRDDs
        JavaPairRDD<String, String> questionPairs = questions.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[0], splits[2]);
        });
        JavaPairRDD<String, Iterable<String>> answerList = answers.mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[1], splits[3]);
        }).groupByKey();

        // Join
        JavaPairRDD<String, Tuple2<String, Iterable<String>>> result = questionPairs.join(answerList);

        // Write the result to the HDFS
        result.saveAsTextFile(outputPath);

		// Close the Spark context
		context.close();
	}
}
