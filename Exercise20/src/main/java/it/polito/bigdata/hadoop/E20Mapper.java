package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.output.MultipleOutputs;

public class E20Mapper extends Mapper<LongWritable, Text, Text, NullWritable> {

    private MultipleOutputs<Text, NullWritable> mo;

    protected void setup(Context context) {
        mo = new MultipleOutputs<>(context);
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        String splits[] = value.toString().split(",");

        if (Double.parseDouble(splits[3]) > 30.0) {
            mo.write("hightemp", value.toString(), NullWritable.get());
        } else {
            mo.write("normaltemp", value.toString(), NullWritable.get());
        }
    }

    protected void cleanup(Context context) throws IOException, InterruptedException {
        mo.close();
    }
}