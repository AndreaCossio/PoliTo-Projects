# Lab 07 - Bike sharing data analysis

In this lab, we analyze historical data about the stations of the bike sharing system of Barcelona. Your task consists in identifying the most “critical” timeslot (day of the week, hour) for each station and store the result in a KML file that can be visualized on a map.

**Inputs**:

1. ```register.csv```: contains the historical information about the number of used and free slots for ~3000 stations from May 2008 to September 2008. Each line of register.csv corresponds to one reading about the situation of one station at a specific timestamp. Each line has the following format:

        stationId   timestamp   #usedslots   #freeslots

    Example:

        23  2008-05-15 19:01:00  5    13

    Pay attention that the first line of register.csv contains the header of the file. Moreover, some of the lines of register.csv contain wrong data due to temporary problems of the monitoring system. Specifically, some lines are characterized by used slots = 0 and free slots = 0. Those lines must be filtered before performing the analysis.

2. ```stations.csv```: contains the description of the stations. Each line has the following format:

        stationId   longitude   latitude name

    Example:

        1   2.180019    41.397978   Gran Via Corts Catalanes

## Exercise 01

Write a single Spark application that identifies the most **critical** timeslot for each station. This analysis can support the planning of the rebalancing operations among stations. Solve the problem by using RDDs.

In this application, each pair ```(day of the week, hour)``` is a **timeslot** and is associated with all the readings associated with that pair, independently of the date. For instance, the timeslot ```(Wednesday, 15)``` corresponds to all the readings made on a Wednesday from 15:00:00 to 15:59:59.

A station ```Si``` is in the critical state when it's full. The **criticality** of a station ```Si``` in the timeslot ```Tj``` is defined as:

![equation](https://latex.codecogs.com/gif.latex?%5Cfrac%7B%5C%26hash%3B%5C%26space%3Bof%5C%26space%3Bfull%5C%26space%3Breadings%5C%26space%3Bof%5C%26space%3B%28Si%2CTj%29%7D%7B%5C%26hash%3B%5C%26space%3Bof%5C%26space%3Breadings%5C%26space%3Bof%5C%26space%3B%28Si%2CTj%29%7D)

Write an application that:

- Computes the criticality value for each pair ```(Si, Tj)```.
- Selects only the pairs having a criticality value greater than a minimum criticality ```threshold```. The minimum criticality ```threshold``` is an argument of the application.
- Selects the most critical timeslot for each station (consider only timeslots with a criticality greater than the minimum criticality ```threshold```). If there are two or more timeslots characterized by the highest criticality value for a station, select only one of those timeslots. Specifically, select the one associated with the earliest hour. If also the hour is the same, consider the lexicographical order of the name of the week day.
- Stores in one single KML file the information about the most critical timeslot for each station. Specifically, the KML file must contain one marker of type Placemark for each pair ```(Si, most critical timeslot for Si)``` characterized by the following features:
    - StationId
    - Day of the week and hour of the critical timeslot
    - Criticality value
    - Coordinates of the station (longitude, latitude)
    
    Do not include in the KML file the stations for which there are no timeslots satisfying the minimum criticality ```threshold```.

The output KML file has the following format:
```kml
<kml xmlns="http://www.opengis.net/kml/2.2">
    <Document>
        <Placemark>
            <name>44</name>
            <ExtendedData>
                <Data name="DayWeek"><value>Mon</value></Data>
                <Data name="Hour"><value>3</value></Data>
                <Data name="Criticality"><value>0.5440729483282675</value></Data>
            </ExtendedData>
            <Point>
                <coordinates>2.189700,41.379047</coordinates>
            </Point>
        </Placemark>
        <Placemark>
            <name>9</name>
            <ExtendedData>
                <Data name="DayWeek"><value>Sat</value></Data>
                <Data name="Hour"><value>10</value></Data>
                <Data name="Criticality"><value>0.5215827338129496</value></Data>
            </ExtendedData>
            <Point>
                <coordinates>2.185294,41.385006</coordinates>
            </Point>
        </Placemark>
    </Document>
</kml>
```

The content of the KML file can be used to show on a Google map the stations characterized by a high criticality value and the associated information. You can visualize the result of you analysis on a map by copy and paste the content of the generated KML file in the form of the following web page http://display-kml.appspot.com/ or you can use one of the many other KML viewers available online.