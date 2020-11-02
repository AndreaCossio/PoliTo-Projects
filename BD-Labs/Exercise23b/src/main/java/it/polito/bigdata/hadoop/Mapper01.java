package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.Mapper;

public class Mapper01 extends Mapper<LongWritable, Text, Text, Text> {

    private String user;

    protected void setup(Context context) {
        user = context.getConfiguration().get("user");
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        String users[] = value.toString().split(",");

        if (user.compareTo(users[0]) != 0) {
            context.write(new Text(users[0]), new Text(users[1]));
        }

        if (user.compareTo(users[1]) != 0) {
            context.write(new Text(users[1]), new Text(users[0]));
        }
    }
}