package it.polito.bigdata.hadoop;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map.Entry;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class E09Mapper extends Mapper<LongWritable, Text, Text, IntWritable> {

    private HashMap<String, Integer> wordCount;

    protected void setup(Context context) {
        wordCount = new HashMap<>();
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        Integer newCount;
        String words[] = value.toString().split("\\s+");

        for (String w : words) {
            
            String cleanWord = w.replaceAll("[^a-zA-Z]", "").toLowerCase();

            newCount = wordCount.get(cleanWord);
            
            if (newCount == null) {
                wordCount.put(cleanWord, 1);
            } else {
                wordCount.put(cleanWord, newCount+1);
            }
        }
    }

    protected void cleanup(Context context) throws IOException, InterruptedException {
        for (Entry<String, Integer> pair : wordCount.entrySet()) {
            context.write(new Text(pair.getKey()), new IntWritable(pair.getValue()));
        }
    }
}