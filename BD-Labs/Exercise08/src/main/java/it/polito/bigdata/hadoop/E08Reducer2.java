package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class E08Reducer2 extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {

    @Override
    protected void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {

        int count = 0;
        double income = 0;

        for (DoubleWritable d : values) {
            income += d.get();
            count ++;
        }

        context.write(key, new DoubleWritable(income / count));
    }
}