package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class E07Reducer extends Reducer<Text, Text, Text, Text> {

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        ArrayList<String> list = new ArrayList<>();

        for (Text t : values) {
            if (!list.contains(t.toString()))
                list.add(t.toString());
        }

        context.write(key, new Text(list.toString()));
    }
}