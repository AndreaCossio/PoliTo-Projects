package it.polito.bigdata.spark;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.api.java.JavaDStream;
import org.apache.spark.streaming.api.java.JavaPairDStream;
import org.apache.spark.streaming.api.java.JavaReceiverInputDStream;
import org.apache.spark.streaming.api.java.JavaStreamingContext;

import scala.Tuple2;
	
public class SparkDriver {
	
	public static void main(String[] args) throws InterruptedException {

		String inputPath = args[0];
        String outputPath = args[1];
	
		// Setup
		SparkConf conf = new SparkConf().setAppName("Exercise47");
		JavaStreamingContext streamingContext = new JavaStreamingContext(conf, Durations.seconds(2));
        JavaSparkContext sparkContext = streamingContext.sparkContext();

		// Read the content of the input file
        JavaRDD<String> records = sparkContext.textFile(inputPath);

        // Create pairs (stationId, name)
        JavaPairRDD<String, String> stationNames = records.mapToPair(x -> {
            String splits[] = x.split("\\t");
            return new Tuple2<>(splits[0], splits[3]);
        });

        JavaReceiverInputDStream<String> readings = streamingContext.socketTextStream("localhost", 9999);

        JavaPairDStream<String, String> fullStationIdTime = readings.filter(x -> {
            return Integer.parseInt(x.split(",")[1]) == 0;
        }).mapToPair(x -> {
            String splits[] = x.split(",");
            return new Tuple2<>(splits[0], splits[3]);
        });

        final JavaDStream<Tuple2<String, String>> stationNameTime = fullStationIdTime.transform(x -> {
            return x.join(stationNames).values();
        });

        stationNameTime.print();
        stationNameTime.dstream().saveAsTextFiles(outputPath, "");

		// Close the streaming context
		streamingContext.start();
		streamingContext.awaitTerminationOrTimeout(120000);
		streamingContext.close();
	}
}
