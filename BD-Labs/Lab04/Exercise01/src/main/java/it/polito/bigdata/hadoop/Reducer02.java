package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

class Reducer02 extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {

    @Override
    protected void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {

    	int count = 0;
        double avg = 0;

        for (DoubleWritable value : values) {
            avg += value.get();
            count++;
        }

        avg /= (double)count;

        context.write(key, new DoubleWritable(avg));
    }
}
