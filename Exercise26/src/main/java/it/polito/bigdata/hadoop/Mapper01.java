package it.polito.bigdata.hadoop;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class Mapper01 extends Mapper<LongWritable, Text, NullWritable, Text> {

    private HashMap<String, Integer> dictionary;

    @SuppressWarnings("deprecation")
    protected void setup(Context context) throws IOException, InterruptedException {
        
        String nextLine;
        BufferedReader dictionaryFile;
		
		dictionary = new HashMap<>();
        dictionaryFile = new BufferedReader(new FileReader(new File(context.getLocalCacheFiles()[0].toUri())));
        
		while ((nextLine = dictionaryFile.readLine()) != null) {
            String split[] = nextLine.split("\t");
            dictionary.put(split[1], Integer.parseInt(split[0]));
		}
	
		dictionaryFile.close();
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        String splits[] = value.toString().split("\\s+");
        StringBuilder result = new StringBuilder();

        for (String s : splits) {
            if (dictionary.get(s) != null) {
                result.append(dictionary.get(s).toString());
                result.append(" ");
            }
        }

        context.write(NullWritable.get(), new Text(result.toString()));
    }
}