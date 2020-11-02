# Lab 01 - Exercise 02

# Retrieving the string
string = input("Insert a string: ")

# Checking length and printing
if len(string) > 2:
    print("'" + string + "' yields '" + string[0] + string[1] + string[len(string) - 2] + string[len(string) - 1] + "'")
else:
    print("")
