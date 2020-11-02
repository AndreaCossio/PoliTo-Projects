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

public class E12Driver extends Configured implements Tool {

    @Override
    public int run(String[] args) throws Exception {

        Path inPath = new Path(args[0]);
        Path outPath = new Path(args[1]);

        // Retrieve configuration
        Configuration conf = this.getConf();
        conf.set("threshold", args[2]);

        // Define job
        Job job = Job.getInstance(conf);
        job.setJobName("Exercise12");

        // Driver
        job.setJarByClass(E12Driver.class);
        FileInputFormat.addInputPath(job, inPath);
        FileOutputFormat.setOutputPath(job, outPath);
        job.setInputFormatClass(KeyValueTextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);

        // Mapper
        job.setMapperClass(E12Mapper.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(DoubleWritable.class);

        // Reducer
        job.setNumReduceTasks(0);

        // Execute the job and wait for completion
        if (job.waitForCompletion(true) == true) {
            return 0;
        } else {
            return 1;
        }
    }

    public static void main(String args[]) throws Exception {
        int res = ToolRunner.run(new Configuration(), new E12Driver(), args);

        System.exit(res);
    }
}