package it.polito.bigdata.hadoop;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

public class DateIncome implements Writable {

	private String date;
	private Integer income;

    public DateIncome() {
        
    }
    
    public DateIncome(String date, Integer income) {
        this.date = date;
        this.income = income;
    }

	public String getDate() {
		return date;
	}

	public Integer getIncome() {
		return income;
	}

	public void setDate(String date) {
		this.date = date;
	}

	public void setIncome(Integer income) {
		this.income = income;
	}

	@Override
	public void readFields(DataInput in) throws IOException {
		income = in.readInt();
		date = in.readUTF();
	}

	@Override
	public void write(DataOutput out) throws IOException {
		out.writeInt(income);
		out.writeUTF(date);
	}

	public String toString() {
		return new String("date:" + date + " income:" + income);
	}

}