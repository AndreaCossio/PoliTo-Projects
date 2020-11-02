package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class Reducer01 extends Reducer<Text, Text, Text, IntWritable> {

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        String result[] = null;
        ArrayList<String> genres = new ArrayList<>();

        for (Text t : values) {
            if (t.toString().startsWith("L:")) {
                genres.add(t.toString().replaceFirst("L:", ""));
            } else {
                result = t.toString().replaceFirst("U:", "").split(",");
            }
        }

        if (genres.contains("Commedia") && genres.contains("Adventure") && result != null) {
            context.write(new Text(result[0]), new IntWritable(Integer.parseInt(result[1])));
        }
    }
}