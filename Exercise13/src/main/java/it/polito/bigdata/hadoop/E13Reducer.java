package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Reducer;

public class E13Reducer extends Reducer<NullWritable, DateIncome, Text, IntWritable> {

    @Override
    protected void reduce(NullWritable key, Iterable<DateIncome> values, Context context) throws IOException, InterruptedException {

        Integer income;
        String date;
        DateIncome top = new DateIncome(null, Integer.MIN_VALUE);

        for (DateIncome d : values) {
            date = d.getDate();
            income = d.getIncome();

            if (income > top.getIncome() || (income == top.getIncome() && date.compareTo(top.getDate()) < 0)) {
                top = new DateIncome(date, income);
            }
        }

        context.write(new Text(top.getDate()), new IntWritable(top.getIncome()));
    }
}