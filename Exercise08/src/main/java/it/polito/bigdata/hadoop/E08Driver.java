package it.polito.bigdata.hadoop;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class E08Driver extends Configured implements Tool {

    @Override
    public int run(String[] args) throws Exception {

        int numReducers = Integer.parseInt(args[0]);
        Path inPath = new Path(args[1]);
        Path outPath = new Path(args[2]);
        Path outPath2 = new Path(args[3]);

        // Retrieve configuration
        Configuration conf = this.getConf();

        // Define job
        Job job = Job.getInstance(conf);
        job.setJobName("Exercise08");

        // Driver
        job.setJarByClass(E08Driver.class);
        FileInputFormat.addInputPath(job, inPath);
        FileOutputFormat.setOutputPath(job, outPath);
        job.setInputFormatClass(KeyValueTextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);

        // Mapper
        job.setMapperClass(E08Mapper1.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(DoubleWritable.class);

        // Reducer
        job.setReducerClass(E08Reducer1.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(DoubleWritable.class);
        job.setNumReduceTasks(numReducers);

        // Execute the job and wait for completion
        if (job.waitForCompletion(true) == true) {
            Job job2 = Job.getInstance(conf);
            job2.setJobName("Exercise08_2");

            // Driver
            job2.setJarByClass(E08Driver.class);
            FileInputFormat.addInputPath(job2, outPath);
            FileOutputFormat.setOutputPath(job2, outPath2);
            job2.setInputFormatClass(KeyValueTextInputFormat.class);
            job2.setOutputFormatClass(TextOutputFormat.class);

            // Mapper2
            job2.setMapperClass(E08Mapper2.class);
            job2.setMapOutputKeyClass(Text.class);
            job2.setMapOutputValueClass(DoubleWritable.class);

            // Reducer2
            job2.setReducerClass(E08Reducer2.class);
            job2.setOutputKeyClass(Text.class);
            job2.setOutputValueClass(DoubleWritable.class);
            job2.setNumReduceTasks(numReducers);

            if (job2.waitForCompletion(true) == true) {
                return 0;
            } else {
                return 1;
            }
        } else {
            return 1;
        }
    }

    public static void main(String args[]) throws Exception {
        int res = ToolRunner.run(new Configuration(), new E08Driver(), args);

        System.exit(res);
    }
}