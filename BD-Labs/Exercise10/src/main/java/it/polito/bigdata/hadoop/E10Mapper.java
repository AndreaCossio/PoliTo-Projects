package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Mapper;

public class E10Mapper extends Mapper<LongWritable, Text, NullWritable, NullWritable> {

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
		context.getCounter(E10Driver.MY_COUNTERS.TOTAL_RECORDS).increment(1);
    }
}