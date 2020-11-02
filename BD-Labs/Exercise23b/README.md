# Friends - Potential list without friends

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

**Output**: list of potential friends (have at least one friend in common) of a given username, but not the ones that are his direct friends. Example (user = User2): 

    User6


```sh
BD-Labs/Exercise23b/target$ hadoop jar Exercise23b-1.jar it.polito.bigdata.hadoop.Driver ./in/ ./out/ User2
```
