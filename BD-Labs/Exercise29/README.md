# User selection (Multiple Inputs)

**Input**: 2 different structured files. The first file has the following format:

    userId,name,surname,gender,yearOfBirth,city,education

while the second file:

    userId,movieGenre

Example:

    User#1,John,Smith,M,1934,New York,Bachelor
    User#2,Paul,Jones,M,1956,Dallas,College
    User#3,Jenny,Smith,F,1934,Philadelphia,Bachelor

    User#1,Commedia
    User#1,Adventure
    User#1,Drama
    User#2,Commedia
    User#2,Crime
    User#3,Commedia
    User#3,Horror
    User#3,Adventure

**Output**: one record (gender, yearOfBirth without removing duplicates) for each user that likes both Commedia and Adventure movies. Example: 

    M	1934
    F	1934

```sh
BD-Labs/Exercise29/target$ hadoop jar Exercise29-1.jar it.polito.bigdata.hadoop.Driver 1 ./in/ ./in2/ ./out/
```
