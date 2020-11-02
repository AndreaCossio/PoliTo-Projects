package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class E11Reducer extends Reducer<Text, Stats, Text, DoubleWritable> {

    @Override
    protected void reduce(Text key, Iterable<Stats> values, Context context) throws IOException, InterruptedException {

        double avg = 0, occ = 0;

        for (Stats s : values) {
            avg += s.getSum();
            occ += s.getCount();
        }

        avg /= occ;

        context.write(key, new DoubleWritable(avg));
    }
}