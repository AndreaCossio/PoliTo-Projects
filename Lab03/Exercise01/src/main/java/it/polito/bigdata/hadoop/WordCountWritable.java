package it.polito.bigdata.hadoop;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

public class WordCountWritable implements Comparable<WordCountWritable>, Writable {

	private Integer count;
	private String word;

	public WordCountWritable() {
	}

	public WordCountWritable(String word, Integer count) {
		this.count = count;
		this.word = word;
	}

	public WordCountWritable(WordCountWritable other) {
		this.count = new Integer(other.getCount());
		this.word = new String(other.getWord());
	}

	public Integer getCount() {
		return count;
	}

	public String getWord() {
		return word;
	}

	public void setCount(Integer count) {
		this.count = count;
	}

	public void setWord(String pair) {
		this.word = pair;
	}

	@Override
	public int compareTo(WordCountWritable other) {
		if (this.count.compareTo(other.getCount()) != 0) {
			return this.count.compareTo(other.getCount());
		} else {
			return this.word.compareTo(other.getWord());
		}
	}

	@Override
	public void readFields(DataInput in) throws IOException {
		count = in.readInt();
		word = in.readUTF();
	}

	@Override
	public void write(DataOutput out) throws IOException {
		out.writeInt(count);
		out.writeUTF(word);
	}

	public String toString() {
		return new String(word + "," + count);
	}
}
