# Friends

**Input**: textual file containing pairs of users representing friendships. Each line has the following format:

    userId,userId

Example:

    User1,User2
    User1,User3
    User1,User4
    User2,User5

**Output**: list of friends of a given username. Example (user = User2): 

    User5 User1

```sh
BD-Labs/Exercise22/target$ hadoop jar Exercise22-1.jar it.polito.bigdata.hadoop.E22Driver ./in/ ./out/ User2
```
