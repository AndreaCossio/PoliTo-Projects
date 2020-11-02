# Friends - Potential list of each user

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

**Output**: list of potential friends (have at least one friend in common) of each user. Example: 

    User1	User2 User5 User4 User3 
    User2	User1 User6 User4 User3 
    User3	User2 User1 User5 User4 
    User4	User2 User1 User5 User3 
    User5	User1 User4 User3 
    User6	User2

```sh
BD-Labs/Exercise25/target$ hadoop jar Exercise25-1.jar it.polito.bigdata.hadoop.Driver 1 ./in/ ./out/ ./out2/
```
