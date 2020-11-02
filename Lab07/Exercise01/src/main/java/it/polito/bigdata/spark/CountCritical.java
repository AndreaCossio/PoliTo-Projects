package it.polito.bigdata.spark;

import java.io.Serializable;

@SuppressWarnings("serial")
public class CountCritical implements Serializable {

    public int tot, critical;

    public CountCritical(int tot, int critical) {
        this.tot = tot;
        this.critical = critical;
    }

    public String toString() {
        return new String("total:" + this.tot + " critical:" + this.critical);
    }

}