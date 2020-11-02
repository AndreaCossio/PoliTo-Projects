package it.polito.bigdata.spark;

import java.io.Serializable;

@SuppressWarnings("serial")
public class TimestampTemperature implements Serializable {

    int timestamp;
    double temperature;

    public TimestampTemperature(int timestamp, double temperature) {
        this.timestamp = timestamp;
        this.temperature = temperature;
    }

    public void setTimestamp(int timestamp) {
        this.timestamp = timestamp;
    }

    public int getTimestamp() {
        return this.timestamp;
    }

    public void setTemperature(double temperature) {
        this.temperature = temperature;
    }

    public double getTemperature() {
        return this.temperature;
    }

    public String toString() {
        return new String(this.timestamp + "," + this.temperature);
    }
}