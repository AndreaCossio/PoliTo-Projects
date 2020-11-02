# Friends - List

**Input**: textual file containing pairs of users representing friendships. Each line has the following format:

    userId,userId

Example:

    User1,User2
    User1,User3
    User1,User4
    User2,User5

**Output**: list of friends of each user. Example: 

    User1	User3 User4 User2 
    User2	User5 User1 
    User3	User1 
    User4	User1 
    User5	User2 

```sh
BD-Labs/Exercise24/target$ hadoop jar Exercise24-1.jar it.polito.bigdata.hadoop.Driver 1 ./in/ ./out/
```
