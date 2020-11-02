package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.HashSet;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class Reducer02 extends Reducer<Text, Text, Text, Text> {

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        StringBuilder result = new StringBuilder();
        HashSet<String> potential = new HashSet<>();

        for (Text t : values) {
            potential.add(t.toString());
        }

        for (String s : potential) {
            result.append(s);
            result.append(" ");
        }

        context.write(new Text(key), new Text(result.toString()));
    }
}