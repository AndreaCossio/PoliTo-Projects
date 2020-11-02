package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.HashSet;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.Mapper;

public class Mapper01 extends Mapper<LongWritable, Text, Text, Text> {

    private HashSet<String> list;

    protected void setup(Context context) {
        list = new HashSet<>();
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        
        String splits[] = value.toString().split(",");
        list.add(value.toString());
        list.add(splits[1] + "," + splits[0]);
    }

    protected void cleanup(Context context) throws IOException, InterruptedException {
        for (String s : list) {
            context.write(new Text(s.split(",")[0]), new Text(s.split(",")[1]));
        }
    }
}