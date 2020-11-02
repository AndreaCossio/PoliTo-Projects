package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.Mapper;

class Mapper01 extends Mapper<LongWritable, Text, Text, IntWritable> {
    
    private String word;
    
    protected void setup(Context context) {
    	word = context.getConfiguration().get("word");
    }
    
	protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        
        String splits[] = value.toString().split("\t");
        String words[] = splits[0].split(" ");

        if (words[0].compareTo(word) == 0 || words[1].compareTo(word) == 0) {
            context.write(new Text(splits[0]), new IntWritable(Integer.parseInt(splits[1])));
        }
    }
}
