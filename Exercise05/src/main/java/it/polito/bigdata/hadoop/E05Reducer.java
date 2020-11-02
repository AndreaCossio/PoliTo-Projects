package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class E05Reducer extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {

    @Override
    protected void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {

        double avg = 0, occ = 0;

        for (DoubleWritable d : values) {
            avg += d.get();
            occ++;
        }

        avg /= occ;

        context.write(key, new DoubleWritable(avg));
    }
}