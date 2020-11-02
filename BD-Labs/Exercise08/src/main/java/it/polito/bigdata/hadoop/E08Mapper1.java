package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class E08Mapper1 extends Mapper<Text, Text, Text, DoubleWritable> {

    @Override
    protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {

        String date[] = key.toString().split("-");
        
        context.write(new Text(date[0] + "-" + date[1]), new DoubleWritable(Double.parseDouble(value.toString())));
    }
}