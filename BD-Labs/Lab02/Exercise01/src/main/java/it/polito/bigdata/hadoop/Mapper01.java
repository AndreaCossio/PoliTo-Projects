package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Mapper;

class Mapper01 extends Mapper<Text, Text, Text, IntWritable> {
    
    private String prefix;
    
    protected void setup(Context context) {
    	prefix = context.getConfiguration().get("prefix");
    }
	
	protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {
        if (key.toString().startsWith(prefix)) {
            context.write(key, new IntWritable(Integer.parseInt(value.toString())));
        }
    }
}
