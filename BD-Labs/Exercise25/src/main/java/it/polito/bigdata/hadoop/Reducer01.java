package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.HashSet;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class Reducer01 extends Reducer<Text, Text, Text, Text> {

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        HashSet<String> friends = new HashSet<>();

        for (Text t : values) {
            friends.add(t.toString());
        }

        for (String s : friends) {
            StringBuilder result = new StringBuilder();

            for (String s2 : friends) {
                if (s.compareTo(s2) != 0) {
                    result.append(s2);
                    result.append(" ");
                }
            }

            if (result.toString().compareTo("") != 0) {
                context.write(new Text(s), new Text(result.toString()));
            }
        }
    }
}