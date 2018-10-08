# Lab 01 - Exercise 01

# First Number
print("Please insert a first number: ", end='')
try:
    a = int(input())
except ValueError:
    print("That didn't seem a number!")
    quit()

# Second Number
print("Please now insert a second number: ", end='')
try:
    b = int(input())
except ValueError:
    print("That didn't seem a number!")
    quit()

# Printing
print("\nTheir sum is: " + str(a + b))
