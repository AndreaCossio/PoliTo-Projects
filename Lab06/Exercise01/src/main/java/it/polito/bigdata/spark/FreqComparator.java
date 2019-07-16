package it.polito.bigdata.spark;

import java.io.Serializable;
import java.util.Comparator;

import scala.Tuple2;

public class FreqComparator implements Comparator<Tuple2<String, Integer>>, Serializable {

	private static final long serialVersionUID = 1L;

	@Override
	public int compare(Tuple2<String, Integer> a, Tuple2<String, Integer> b) {
		return a._2().compareTo(b._2());
	}
}