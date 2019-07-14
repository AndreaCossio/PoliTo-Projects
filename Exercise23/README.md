# Friends - Potential list

**Input**: textual file containing pairs of users representing friendships. Each line has the following format:

    userId,userId

Example:

    User1,User2
    User1,User3
    User1,User4
    User2,User3
    User2,User4
    User2,User5
    User5,User6

**Output**: list of potential friends (have at least one friend in common) of a given username. Example (user = User2): 

    User1 User6 User4 User3 


```sh
BD-Labs/Exercise23/target$ hadoop jar Exercise23-1.jar it.polito.bigdata.hadoop.Driver ./in/ ./out/ User2
```
