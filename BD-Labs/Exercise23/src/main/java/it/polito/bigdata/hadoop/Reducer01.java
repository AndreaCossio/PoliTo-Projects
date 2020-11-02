package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.HashSet;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class Reducer01 extends Reducer<Text, Text, Text, NullWritable> {

    private HashSet<String> finalList;
    private String user;

    protected void setup(Context context) {
        user = context.getConfiguration().get("user");
        finalList = new HashSet<>();
    }

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        boolean isFriend = false;
        HashSet<String> partialList = new HashSet<>();

        for (Text t : values) {
            if (user.compareTo(t.toString()) == 0) {
                isFriend = true;
            } else {
                partialList.add(t.toString());
            }
        }

        if (isFriend && partialList.size() > 0) {
            for (String s : partialList) {
                finalList.add(s);
            }
        }
    }

    protected void cleanup(Context context) throws IOException, InterruptedException {

        StringBuilder result = new StringBuilder();

        for (String s : finalList) {
            result.append(s);
            result.append(" ");
        }

        context.write(new Text(result.toString()), NullWritable.get());
    }
}