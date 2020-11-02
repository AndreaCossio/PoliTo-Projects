package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map.Entry;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.Mapper;

public class E11Mapper extends Mapper<LongWritable, Text, Text, Stats> {

    private HashMap<String, Stats> stats;

    protected void setup(Context context) {
        stats = new HashMap<>();
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        String fields[] = value.toString().split(",");
        Stats s;

        s = stats.get(fields[0]);

        if (s == null) {
            stats.put(fields[0], new Stats(1, Double.parseDouble(fields[2])));
        } else {
            s.setCount(s.getCount() + 1);
            s.setSum(s.getSum() + Double.parseDouble(fields[2]));
            stats.put(fields[0], s);
        }        
    }

    protected void cleanup(Context context) throws IOException, InterruptedException {

        for (Entry<String, Stats> e : stats.entrySet()) {
            context.write(new Text(e.getKey()), e.getValue());
        }
    }
}