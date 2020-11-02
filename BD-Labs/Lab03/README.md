# Lab 03

## Exercise 01 - Recommendation system

In this exercise, we try to build a very basic version of a recommending system. Your goal is to find the top 100 pairs of products most often reviewed (and so probably bought) together.
In this exercise, we consider two products as reviewed (i.e., bought) together if they appear in the same line of the input transposed file. We ignore temporal constraints, so even if a decade or a thousand products have passed between the two reviews, we count the pair, as it represents anyway the tastes of a single user.