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

        for (int i = 0; i < words.length - 1; i++) {
            String bigram = words[i] + " " + words[i+1];

            if (bigram.matches("[a-zA-Z0-9]+ [a-zA-Z0-9]+")) {
                context.write(new Text(bigram.toLowerCase()), new IntWritable(1));
            }
        }
    }
}