package it.polito.bigdata.hadoop;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class E13bDriver extends Configured implements Tool {

    @Override
    public int run(String[] args) throws Exception {

        int numReducers = Integer.parseInt(args[0]);
        Path inPath = new Path(args[1]);
        Path outPath = new Path(args[2]);

        // Retrieve configuration
        Configuration conf = this.getConf();
        conf.set("topK", args[3]);

        // Define job
        Job job = Job.getInstance(conf);
        job.setJobName("Exercise13b");

        // Driver
        job.setJarByClass(E13bDriver.class);
        FileInputFormat.addInputPath(job, inPath);
        FileOutputFormat.setOutputPath(job, outPath);
        job.setInputFormatClass(KeyValueTextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);

        // Mapper
        job.setMapperClass(E13bMapper.class);
        job.setMapOutputKeyClass(NullWritable.class);
        job.setMapOutputValueClass(DateIncome.class);

        // Reducer
        job.setReducerClass(E13bReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        job.setNumReduceTasks(numReducers);

        // Execute the job and wait for completion
        if (job.waitForCompletion(true) == true) {
            return 0;
        } else {
            return 1;
        }
    }

    public static void main(String args[]) throws Exception {
        int res = ToolRunner.run(new Configuration(), new E13bDriver(), args);

        System.exit(res);
    }
}