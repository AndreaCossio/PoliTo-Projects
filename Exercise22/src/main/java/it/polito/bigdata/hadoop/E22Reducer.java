package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class E22Reducer extends Reducer<NullWritable, Text, NullWritable, Text> {

    @Override
    protected void reduce(NullWritable key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        ArrayList<String> users = new ArrayList<>();
        StringBuilder result = new StringBuilder();

        for (Text t : values) {
            if (!users.contains(t.toString())) {
                users.add(t.toString());
            }
        }

        for (String s : users) {
            result.append(s);
            result.append(" ");
        }

        context.write(NullWritable.get(), new Text(result.toString()));
    }
}