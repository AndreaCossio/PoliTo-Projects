package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import it.polito.bigdata.hadoop.DateIncome;

public class E13Mapper extends Mapper<Text, Text, NullWritable, DateIncome> {

    private DateIncome top;

    protected void setup(Context context) {
        top = new DateIncome(null, Double.MIN_VALUE);
    }

    @Override
    protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {

        String date = key.toString();

        double income = Double.parseDouble(value.toString());
        
        if (income > top.getIncome() || (income == top.getIncome() && date.compareTo(top.getDate()) < 0)) {
            top = new DateIncome(date, income);
        }
    }

    protected void cleanup(Context context) throws IOException, InterruptedException {
        context.write(NullWritable.get(), top);
    }
}