package it.polito.bigdata.hadoop;

import java.util.Vector;

public class TopKVector<T extends Comparable<T>> {

	private Integer k;
	private Vector<T> localTopK;

	public TopKVector(int k) {
		this.k = k;
		this.localTopK = new Vector<T>();
	}

	public int getK() {
		return this.k;
	}

	public Vector<T> getLocalTopK() {
		return this.localTopK;
	}

	public void updateWithNewElement(T currentElement) {
		if (localTopK.size() < k) {
			localTopK.addElement(currentElement);
			sortAfterInsertNewElement();
		} else {
			if (currentElement.compareTo(localTopK.elementAt(k - 1)) > 0) {
				localTopK.setElementAt(currentElement, k - 1);
				sortAfterInsertNewElement();
			}
		}
	}

	private void sortAfterInsertNewElement() {
		T swap;
		for (int pos = localTopK.size() - 1; pos > 0 && localTopK.elementAt(pos).compareTo(localTopK.elementAt(pos - 1)) > 0; pos--) {
			swap = localTopK.elementAt(pos);
			localTopK.setElementAt(localTopK.elementAt(pos - 1), pos);
			localTopK.setElementAt(swap, pos - 1);
		}
	}
}
