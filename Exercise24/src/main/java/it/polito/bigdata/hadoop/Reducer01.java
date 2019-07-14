package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class Reducer01 extends Reducer<Text, Text, Text, Text> {

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        StringBuilder result = new StringBuilder();

        for (Text t : values) {
            result.append(t.toString());
            result.append(" ");
        }

        context.write(key, new Text(result.toString()));
    }
}