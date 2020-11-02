package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

class Mapper02 extends Mapper<Text, Text, NullWritable, WordCountWritable> {

    private TopKVector<WordCountWritable> localTopK;

    protected void setup(Context context) {
        int k = Integer.parseInt(context.getConfiguration().get("k"));
        localTopK = new TopKVector<>(k);
    }

	protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {
        localTopK.updateWithNewElement(new WordCountWritable(key.toString(), Integer.parseInt(value.toString())));
    }

    protected void cleanup(Context context) throws IOException, InterruptedException {
        for (WordCountWritable pair : localTopK.getLocalTopK()) {
            context.write(NullWritable.get(), pair);
        }
    }
}
