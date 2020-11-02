package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class Reducer01 extends Reducer<Text, Text, NullWritable, Text> {

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        StringBuilder question = new StringBuilder();
        ArrayList<String> answers = new ArrayList<>();

        question.append(key.toString().split(",")[0]);
        question.append(",");

        for (Text t : values) {
            if (t.toString().startsWith("A")) {
                answers.add(t.toString());
            } else {
                question.append(t.toString());
                question.append(",");
            }
        }

        for (String a : answers) {
            context.write(NullWritable.get(), new Text(question.toString() + a));
        }
    }
}