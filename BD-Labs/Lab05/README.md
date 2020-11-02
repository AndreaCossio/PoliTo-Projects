# Lab 05

## Exercise 01 - Filtering

If you completed Lab 1, you should now have (at least one) huge files with the word frequencies in the amazon food reviews, in the format ```word\tfreq```, where ```freq``` is an integer. Your task is to write a **Spark** application to filter these results, analyze the filtered data and compute some statistics on them.

The first filter you should implement is the following:

- keep only the lines containing words that start with a ```prefix``` (a string) that is passed as a command-line parameter. The result is the set of lines (```word\tfreq```) that satisfy the filtering operation.

Print on the standard output:

- the number of selected lines
- the maximum frequency (```maxfreq```) among the ones of the selected lines (i.e., the maximum value of ```freq``` in the lines obtained by applying the filter).

In the second part of your application, among the lines selected by the first filter, you have to apply another filter to select only the most frequent words. Specifically, your application must select those lines that contain words with a frequency (```freq```) greater than ```80%``` of the maximum frequency (```maxfreq```) computed before.

Hence, implement the following filter:

- keep only the lines with a frequency ```freq``` greater than ```0.8 * maxfreq```.

Finally, perform the following operations on the selected lines (the ones selected by applying both filters):

- count the number of selected lines and print this number on the standard output
- save the selected words (without frequency) in an output folder for inspecting the results