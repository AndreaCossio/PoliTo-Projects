package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class E07Mapper extends Mapper<Text, Text, Text, Text> {

    @Override
    protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {

        String words[] = value.toString().split("\\s+");

        for (String w : words) {
            context.write(new Text(w.replaceAll("[^a-zA-Z]", "").toLowerCase()), key);
        }
    }
}