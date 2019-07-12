package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class E06Reducer extends Reducer<Text, DoubleWritable, Text, Text> {

    @Override
    protected void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {

        int i = 0;
        double min = 0, max = 0;

        for (DoubleWritable d : values) {
            double val = d.get();
            if (i == 0) {
                min = val;
                max = min;
                i++;
            } else {
                if (val > max) {
                    max = val;
                }
                if (val < min) {
                    min = val;
                }
            }
        }

        context.write(key, new Text("max="+max+"_min="+min));
    }
}