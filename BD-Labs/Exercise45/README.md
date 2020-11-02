# Misleading profile selection

**Input**: a CSV file containing the list of movies watched by a user with the following format:

    userId,movieId,startTime,endTime

Example:

    user1,movie1,201606061500,201606061650
    user1,movie3,201606061800,201606061834
    user1,movie4,201609061500,201609061605
    user1,movie5,201610061100,201610061450
    user2,movie6,201610081800,201610081845
    user2,movie3,201610091800,201610091834
    user2,movie4,201611051100,201611051105
    user2,movie7,201611081700,201610081745
    user2,movie3,201611091700,201610091734
    user2,movie4,201612051700,201611051705

A CSV file containing user preferences of movie genre with the following format:

    userId,genre

Example:

    user1,Animation
    user1,Comedy
    user2,Action

A CSV file containing the informations of movies with the following format:

    movieId,title,genre

Example:

    movie1,Toy Story (1995),Animation
    movie2,Jumanji (1995),Adventure
    movie3,Grumpier Old Men (1995),Comedy
    movie4,Waiting to Exhale (1995),Comedy
    movie5,Father of the Bride Part II (1995),Comedy
    movie6,Heat (1995),Action
    movie7,Sabrina (1995),Comedy
    movie8,Tom and Huck (1995),Adventure
    movie9,Sudden Death (1995),Action
    movie10,GoldenEye (1995),Action

**Output**: the list of genres that are associated to a movie that has been seen at least 5 times by a misleading user profile. Example (thr = 0.5):

    (user2,Comedy)

```sh
BD-Labs/Exercise45/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise45-1.jar ./in/input.txt ./in/preferences.txt ./in/movies.txt ./out/ 0.5
```
