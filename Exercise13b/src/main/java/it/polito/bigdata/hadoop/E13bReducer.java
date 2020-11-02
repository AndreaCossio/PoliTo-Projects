package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class E13bReducer extends Reducer<NullWritable, DateIncome, Text, IntWritable> {

    @Override
    protected void reduce(NullWritable key, Iterable<DateIncome> values, Context context) throws IOException, InterruptedException {

        TopKVector<DateIncome> globalTopK = new TopKVector<>(Integer.parseInt(context.getConfiguration().get("topK")));

        for (DateIncome d : values) {
            globalTopK.updateWithNewElement(new DateIncome(d));
        }

        for (DateIncome d : globalTopK.getLocalTopK()) {
            context.write(new Text(d.getDate()), new IntWritable(d.getIncome()));
        }
    }
}