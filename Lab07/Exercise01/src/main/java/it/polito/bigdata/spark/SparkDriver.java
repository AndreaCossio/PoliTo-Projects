package it.polito.bigdata.spark;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.URI;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;

import scala.Tuple2;

public class SparkDriver {

    public static void main(String[] args) {

        String inputPath = args[0];
        String inputPath2 = args[1];
        String outputPath = args[2];
        Double threshold = Double.parseDouble(args[3]);

        // Setup
        SparkConf conf = new SparkConf().setAppName("Lab07_Ex1");
        JavaSparkContext context = new JavaSparkContext(conf);

        // Read the content of the input files
        JavaRDD<String> records = context.textFile(inputPath);
        JavaRDD<String> stations = context.textFile(inputPath2);

        // Create pairs (stationId-day_hour, Count(tot, critical))
        JavaPairRDD<String, CountCritical> countCriticalitiesFlat = records.filter(x -> {
            if (x.startsWith("station")) {
                return false;
            } else {
                String splits[] = x.split("\\t");
                if (Integer.parseInt(splits[2]) == 0 && Integer.parseInt(splits[3]) == 0) {
                    return false;
                } else {
                    return true;
                }
            }
        }).mapToPair(x -> {

            String splits[] = x.split("\\t");

            String timestamp[] = splits[1].split(" ");
            String date = timestamp[0];
            String hour = timestamp[1].split(":")[0];

            String key = splits[0] + "-" + MyDate.DayOfTheWeek(date) + "_" + hour;

            if (Integer.parseInt(splits[3]) == 0) {
                return new Tuple2<>(key, new CountCritical(1, 1));
            } else {
                return new Tuple2<>(key, new CountCritical(1, 0));
            }
        });

        // Group by stationId-day_hour and reduce count critical
        JavaPairRDD<String, CountCritical> countCriticalities = countCriticalitiesFlat.reduceByKey((a, b) -> {
            return new CountCritical(a.tot + b.tot, a.critical + b.critical);
        });

        // Create pairs (stationId-day_hour, criticality)
        JavaPairRDD<String, Double> criticalities = countCriticalities.mapValues(x -> {
            return (double) x.critical / x.tot;
        }).filter(x -> x._2() > threshold);

        // Create pairs (stationId, most critical timeslot day_hour-criticality)
        JavaPairRDD<String, String> stationCriticalTime = criticalities.mapToPair(x -> {

            String splits[] = x._1().split("-");

            String sId = splits[0];
            String timeslot = splits[1];

            return new Tuple2<>(sId, timeslot + "-" + x._2().toString());
        }).reduceByKey((a, b) -> {

            Double aCrit = Double.parseDouble(a.split("-")[1]);
            Double bCrit = Double.parseDouble(b.split("-")[1]);

            Integer aH = Integer.parseInt(a.split("-")[0].split("_")[1]);
            Integer bH = Integer.parseInt(b.split("-")[0].split("_")[1]);

            if (aCrit == bCrit) {
                if (aH == bH) {
                    if (a.compareTo(b) > 0) {
                        return b;
                    } else {
                        return a;
                    }
                } else {
                    if (aH > bH) {
                        return b;
                    } else {
                        return a;
                    }
                }
            } else {
                if (aCrit > bCrit) {
                    return a;
                } else {
                    return b;
                }
            }
        });

        // Create pairs (stationId, long,lat)
        JavaPairRDD<String, String> stationsMap = stations.filter(x -> !x.startsWith("id")).mapToPair(x -> {
            String splits[] = x.split("\\t");
            return new Tuple2<>(splits[0], splits[1] + "," + splits[2]);
        });

        JavaPairRDD<String, Tuple2<String, String>> joinedStationCritical = stationCriticalTime.join(stationsMap);

        /*
         * STORE KML
         */
        // Store in resultKML one String, representing a KML marker, for each station
        // with a critical timeslot
        // JavaRDD<String> resultKML = ;
        List<String> result = joinedStationCritical.map(x -> {

            String time = x._2()._1();
            String coord = x._2()._2();

            String sId = x._1();

            String timeslot = time.split("-")[0];
            Double criticality = Double.parseDouble(time.split("-")[1]);

            String day = timeslot.split("_")[0];
            String hour = timeslot.split("_")[1];

            return "<Placemark>" + "<name>" + sId + "</name>" + "<ExtendedData>" + "<Data name=\"DayWeek\"><value>"
                    + day + "</value></Data>" + "<Data name=\"Hour\"><value>" + hour + "</value></Data>"
                    + "<Data name=\"Criticality\"><value>" + criticality + "</value></Data>" + "</ExtendedData>"
                    + "<Point>" + "<coordinates>" + coord + "</coordinates>" + "</Point>" + "</Placemark>";
        }).collect();

        Configuration confHadoop = new Configuration();

        try {
            URI uri = URI.create(outputPath);
            FileSystem file = FileSystem.get(uri, confHadoop);
            FSDataOutputStream outputFile = file.create(new Path(uri));
            BufferedWriter bOutFile = new BufferedWriter(new OutputStreamWriter(outputFile, "UTF-8"));

            // Header
            bOutFile.write("<kml xmlns=\"http://www.opengis.net/kml/2.2\"><Document>");
            bOutFile.newLine();

            // Markers
            for (String lineKML : result) {
                bOutFile.write(lineKML);
                bOutFile.newLine();
            }

            // Footer
            bOutFile.write("</Document></kml>");
            bOutFile.newLine();

            bOutFile.close();
            outputFile.close();

        } catch (IOException e1) {
            e1.printStackTrace();
        }

        // Close the Spark context
        context.close();
    }
}
