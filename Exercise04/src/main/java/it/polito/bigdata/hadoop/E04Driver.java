package it.polito.bigdata.hadoop;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class E04Driver extends Configured implements Tool {

    @Override
    public int run(String[] args) throws Exception {

        int exitCode, numReducers = Integer.parseInt(args[0]);
        Path inPath = new Path(args[1]);
        Path outPath = new Path(args[2]);

        // Retrieve configuration
        Configuration conf = this.getConf();
        conf.set("threshold", args[3]);

        // Define job
        Job job = Job.getInstance(conf);
        job.setJobName("Exercise04");

        // Driver
        job.setJarByClass(E04Driver.class);
        FileInputFormat.addInputPath(job, inPath);
        FileOutputFormat.setOutputPath(job, outPath);
        job.setInputFormatClass(KeyValueTextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);

        // Mapper
        job.setMapperClass(E04Mapper.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);

        // Reducer
        job.setReducerClass(E04Reducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);
        job.setNumReduceTasks(numReducers);

        // Execute the job and wait for completion
        if (job.waitForCompletion(true) == true)
            exitCode = 0;
        else
            exitCode = 1;

        return exitCode;
    }

    public static void main(String args[]) throws Exception {
        int res = ToolRunner.run(new Configuration(), new E04Driver(), args);

        System.exit(res);
    }
}