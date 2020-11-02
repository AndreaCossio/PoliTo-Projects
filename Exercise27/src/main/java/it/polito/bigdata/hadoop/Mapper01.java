package it.polito.bigdata.hadoop;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class Mapper01 extends Mapper<LongWritable, Text, NullWritable, Text> {

    private ArrayList<String> rules;

    @SuppressWarnings("deprecation")
    protected void setup(Context context) throws IOException, InterruptedException {
        
        String nextLine;
        BufferedReader dictionaryFile;
		
		rules = new ArrayList<>();
        dictionaryFile = new BufferedReader(new FileReader(new File(context.getLocalCacheFiles()[0].toUri())));
        
		while ((nextLine = dictionaryFile.readLine()) != null) {
            rules.add(nextLine);
		}
	
		dictionaryFile.close();
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        String category = "Unknown";
        String splits[] = value.toString().split(",");

        for (String rule : rules) {
            String ruleSplits[] = rule.split(" ");

            if (ruleSplits[0].compareTo("Gender=" + splits[3]) == 0 && ruleSplits[2].compareTo("YearOfBirth=" + splits[4]) == 0) {
                category = ruleSplits[4];
            }
        }

        context.write(NullWritable.get(), new Text(value.toString() + "," + category));
    }
}