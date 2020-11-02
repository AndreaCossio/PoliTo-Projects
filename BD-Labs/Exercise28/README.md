# Mapping questions and answers (Multiple Inputs)

**Input**: 2 different structured files. The first file has the following format:

    questionId,timestamp,textQuestion

while the second file:

    answerId,questionId,timestamp,textAnswer

Example:

    Q1,2015-01-01,What is ..?
    Q2,2015-01-03,Who invented ..

    A1,Q1,2015-01-02,It is ..
    A2,Q2,2015-01-03,John Smith
    A3,Q1,2015-01-05,I think it is ..

**Output**: one line for each pair (question, answer). Example: 

    Q1,What is ..?,A3,I think it is ..
    Q1,What is ..?,A1,It is ..
    Q2,Who invented ..,A2,John Smith

```sh
BD-Labs/Exercise28/target$ hadoop jar Exercise28-1.jar it.polito.bigdata.hadoop.Driver 1 ./in/ ./in2/ ./out/
```
