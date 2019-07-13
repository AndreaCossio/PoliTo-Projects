package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class E12Mapper extends Mapper<Text, Text, Text, DoubleWritable> {

    @Override
    protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {

        Double pm = Double.parseDouble(value.toString());

        if (pm < Double.parseDouble(context.getConfiguration().get("threshold"))) {
            context.write(key, new DoubleWritable(pm));
        }
    }
}