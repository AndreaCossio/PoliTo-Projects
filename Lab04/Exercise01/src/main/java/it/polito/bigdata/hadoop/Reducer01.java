package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.HashMap;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

class Reducer01 extends Reducer<Text, Text, Text, DoubleWritable> {

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
    	
        double avg = 0;
        String[] fields;
        HashMap<String, Double> avgScores = new HashMap<>();
        
        for (Text value : values) {
            fields = value.toString().split("-");
            avgScores.put(fields[0], Double.parseDouble(fields[1]));
        }
        
        avg = avgScores.values().stream().mapToDouble(Double::valueOf).average().getAsDouble();

        for (HashMap.Entry<String, Double> e : avgScores.entrySet()) {
            context.write(new Text(e.getKey()), new DoubleWritable(e.getValue()-avg));
        }
    }
}
