package it.polito.bigdata.hadoop;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.Counter;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class E10Driver extends Configured implements Tool {

    public static enum MY_COUNTERS {
		TOTAL_RECORDS
	}

    @Override
    public int run(String[] args) throws Exception {

        Path inPath = new Path(args[0]);
        Path outPath = new Path(args[1]);

        // Retrieve configuration
        Configuration conf = this.getConf();

        // Define job
        Job job = Job.getInstance(conf);
        job.setJobName("Exercise10");

        // Driver
        job.setJarByClass(E10Driver.class);
        FileInputFormat.addInputPath(job, inPath);
        FileOutputFormat.setOutputPath(job, outPath);
        job.setInputFormatClass(TextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);

        // Mapper
        job.setMapperClass(E10Mapper.class);
        job.setMapOutputKeyClass(NullWritable.class);
        job.setMapOutputValueClass(NullWritable.class);

        // Reducer
        job.setNumReduceTasks(0);

        // Execute the job and wait for completion
        if (job.waitForCompletion(true) == true) {
            Counter records = job.getCounters().findCounter(MY_COUNTERS.TOTAL_RECORDS);
            System.out.println("Total number of records = " + records.getValue());
            return 0;
        } else {
            return 1;
        }
    }

    public static void main(String args[]) throws Exception {
        int res = ToolRunner.run(new Configuration(), new E10Driver(), args);

        System.exit(res);
    }
}