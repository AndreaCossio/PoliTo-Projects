package it.polito.bigdata.hadoop;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

public class Stats implements Writable {

    private int count = 0;
    private double sum = 0;

    public Stats() {
        
    }
    
    public Stats(int count, double sum) {
        this.count = count;
        this.sum = sum;
    }

	public double getSum() {
		return sum;
	}

	public int getCount() {
		return count;
	}

	public void setSum(double sum) {
		this.sum = sum;
	}

	public void setCount(int count) {
		this.count = count;
	}

	@Override
	public void readFields(DataInput in) throws IOException {
		sum = in.readDouble();
		count = in.readInt();
	}

	@Override
	public void write(DataOutput out) throws IOException {
		out.writeDouble(sum);
		out.writeInt(count);
	}

	public String toString() {
		return new String("" + (double) sum / count);
	}

}