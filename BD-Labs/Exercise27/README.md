# Categorization rules

**Input**: CSV file with the following format:

    userId,name,surname,gender,yearofbirth,city,education

Example:

    User#1,John,Smith,M,1934,New York,Bachelor
    User#2,Paul,Jones,M,1956,Dallas,College
    User#3,Jenny,Smith,F,1934,Philadelphia,Bachelor
    User#4,Laura,White,F,1926,New York,Doctorate

Small file with a set of business rules (mutually exclusive) to assign users to a category. Example:

    Gender=M and YearOfBirth=1934 -> Category#1
    Gender=M and YearOfBirth=1956 -> Category#3
    Gender=F and YearOfBirth=1934 -> Category#2
    Gender=F and YearOfBirth=1956 -> Category#3

**Output**: user information + the category ("Unknown" if none). Example: 

    User#1,John,Smith,M,1934,New York,Bachelor,Category#1
    User#2,Paul,Jones,M,1956,Dallas,College,Category#3
    User#3,Jenny,Smith,F,1934,Philadelphia,Bachelor,Category#2
    User#4,Laura,White,F,1926,New York,Doctorate,Unknown

```sh
BD-Labs/Exercise27/target$ hadoop jar Exercise27-1.jar it.polito.bigdata.hadoop.Driver ./in/ ./out/ ../rules.txt
```
