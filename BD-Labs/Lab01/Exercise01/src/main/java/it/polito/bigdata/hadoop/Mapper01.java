package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class Mapper01 extends Mapper<LongWritable, Text, Text, IntWritable> {

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        String words[] = value.toString().split("\\s+");

        for (String w : words) {
            context.write(new Text(w.replaceAll("[^a-zA-Z]", "").toLowerCase()), new IntWritable(1));
        }
    }
}