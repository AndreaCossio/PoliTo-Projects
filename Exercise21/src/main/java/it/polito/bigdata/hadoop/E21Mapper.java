package it.polito.bigdata.hadoop;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class E21Mapper extends Mapper<LongWritable, Text, NullWritable, Text> {

    private ArrayList<String> stopwords;

    @SuppressWarnings("deprecation")
    protected void setup(Context context) throws IOException, InterruptedException {
        
        String nextLine;
        Path[] PathsCachedFiles;
        BufferedReader fileStopWords;
		
		stopwords = new ArrayList<String>();
		PathsCachedFiles = context.getLocalCacheFiles();
        fileStopWords = new BufferedReader(new FileReader(new File(PathsCachedFiles[0].toUri())));
        
		while ((nextLine = fileStopWords.readLine()) != null) {
			stopwords.add(nextLine);
		}
	
		fileStopWords.close();
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        String splits[] = value.toString().split("\\s+");
        String result = "";

        for (String s : splits) {
            if (!stopwords.contains(s)) {
                result = result.concat(s + " ");
            }
        }

        context.write(NullWritable.get(), new Text(result));
    }
}