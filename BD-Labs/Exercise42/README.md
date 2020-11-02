# Mapping questions and answers

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

**Output**: one line for each question with the list of answers. Example: 

    (Q1,(What is ..?,[It is .., I think it is ..]))
    (Q2,(Who invented ..,[John Smith]))


```sh
BD-Labs/Exercise42/target$ spark-submit --class it.polito.bigdata.spark.SparkDriver --deploy-mode client --master local Exercise42-1.jar ./in/questions.txt ./in/answers.txt ./out/
```
