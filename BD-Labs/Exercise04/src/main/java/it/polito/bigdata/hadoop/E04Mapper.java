package it.polito.bigdata.hadoop;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class E04Mapper extends Mapper<Text, Text, Text, Text> {

    @Override
    protected void map(Text key, Text value, Context context) throws IOException, InterruptedException {

        String zoneAndDate[] = key.toString().split(",");

        Double pm = Double.parseDouble(value.toString());

        if (pm > Double.parseDouble(context.getConfiguration().get("threshold"))) {
            context.write(new Text(zoneAndDate[0]), new Text(zoneAndDate[1]));
        }
    }
}