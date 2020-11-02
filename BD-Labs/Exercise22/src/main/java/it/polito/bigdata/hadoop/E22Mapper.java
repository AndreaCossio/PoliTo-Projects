package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Mapper;

public class E22Mapper extends Mapper<LongWritable, Text, NullWritable, Text> {

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        String users[] = value.toString().split(",");
        String user = context.getConfiguration().get("user");

        if (user.compareTo(users[0]) == 0) {
            context.write(NullWritable.get(), new Text(users[1]));
        }

        if (user.compareTo(users[1]) == 0) {
            context.write(NullWritable.get(), new Text(users[0]));
        }
    }
}