# Lab 06

## Exercise 01 - PairRDD and Top elements

Write a single **Spark** application that:

- Transposes the original Amazon food dataset, obtaining a PairRDD of the type:
    (```user_id```, list of the ```product_ids``` reviewed by ```user_id```)
- Counts the frequencies of all the pairs of products reviewed together;
- Writes on the output folder all the pairs of products that appear more than once and their frequencies. The pairs of products must be sorted by decreasing frequency.
- Writes to standard output the top 10 most frequent pairs and their frequency

The input *Amazon food dataset* lists reviews in the following format:

    Id,ProductId,UserId,ProfileName,HelpfulnessNumerator,HelpfulnessDenominator,Score,Time,Summary,Text
