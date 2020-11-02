package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

class Reducer02 extends Reducer<NullWritable, WordCountWritable, Text, IntWritable> {

    @Override
    protected void reduce(NullWritable key, Iterable<WordCountWritable> values, Context context) throws IOException, InterruptedException {

        int k = Integer.parseInt(context.getConfiguration().get("k"));
        TopKVector<WordCountWritable> globalTopK = new TopKVector<>(k);

        for (WordCountWritable pair : values) {
            globalTopK.updateWithNewElement(new WordCountWritable(pair));
        }

        for (WordCountWritable pair : globalTopK.getLocalTopK()) {
            context.write(new Text(pair.getWord()), new IntWritable(pair.getCount()));
        }
    	
    }
}
