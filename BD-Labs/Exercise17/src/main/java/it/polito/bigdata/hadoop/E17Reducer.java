package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class E17Reducer extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {

    @Override
    protected void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {

        double max = Double.MIN_VALUE;

        for (DoubleWritable d : values) {
            if (d.get() > max) {
                max = d.get();
            }
        }

        context.write(key, new DoubleWritable(max));
    }
}