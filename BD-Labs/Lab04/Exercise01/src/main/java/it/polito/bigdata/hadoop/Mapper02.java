package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

class Mapper02 extends Mapper<Text, Text, Text, DoubleWritable> {

    protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {
        context.write(key, new DoubleWritable(Double.parseDouble(value.toString())));
    }
}
