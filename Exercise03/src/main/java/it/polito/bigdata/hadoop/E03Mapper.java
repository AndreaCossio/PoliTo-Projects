package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class E03Mapper extends Mapper<Text, Text, Text, IntWritable> {

    @Override
    protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {

        String sensorId = key.toString().split(",")[0];

        Double pm = Double.parseDouble(value.toString());

        if (pm > Double.parseDouble(context.getConfiguration().get("threshold"))) {
            context.write(new Text(sensorId), new IntWritable(1));
        }
    }
}