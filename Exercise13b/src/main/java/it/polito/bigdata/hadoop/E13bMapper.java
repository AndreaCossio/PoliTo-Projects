package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import it.polito.bigdata.hadoop.DateIncome;

public class E13bMapper extends Mapper<Text, Text, NullWritable, DateIncome> {

    private TopKVector<DateIncome> top;

    protected void setup(Context context) {
        top = new TopKVector<>(Integer.parseInt(context.getConfiguration().get("topK")));
    }

    @Override
    protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {
        top.updateWithNewElement(new DateIncome(key.toString(), Integer.parseInt(value.toString())));
    }

    protected void cleanup(Context context) throws IOException, InterruptedException {
        for (DateIncome d : top.getLocalTopK()) {
            context.write(NullWritable.get(), d);
        }
    }
}