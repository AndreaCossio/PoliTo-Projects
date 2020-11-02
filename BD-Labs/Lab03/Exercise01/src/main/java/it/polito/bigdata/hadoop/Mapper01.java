package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

class Mapper01 extends Mapper<LongWritable, Text, Text, IntWritable> {

	protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

		ArrayList<String> words = new ArrayList<>(Arrays.asList(value.toString().split(",")));

		words.remove(0);
		
		words.sort((a, b) -> a.compareTo(b));
		
		for (int i = 0; i < words.size(); i++) {
			for (int j = i + 1; j < words.size(); j++) {
				if (!words.get(i).equals(words.get(j))) {
					String pair = words.get(i) + " " + words.get(j);
					context.write(new Text(pair), new IntWritable(1));
				}
			}
		}
	}
}
