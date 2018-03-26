# Lab 02 - Exercise 03

# In order to read parameters
import sys

# List of tasks
tasks = []
num = -1

# Reading from file
with open(sys.argv[1], "r+") as file:
    for line in file:
        tasks.append(line.strip("\n"))
file.close()

# Main loop
while num != 5:

    # Printing menu
    print("""Insert the number corresponding to the action you want to perform:
            1. Insert a new task
            2. Remove a task (by typing its content exactly)
            3. Remove all existing tasks that match a substring
            4. Show all existing tasks, sorted in alphabetic order
            5. Close the program""")

    # Catching exceptions and performing different actions
    try:
        num = int(input("> "))
        if num == 1:
            new_task = input("\nPlease insert the task to insert: ")
            tasks.append(new_task)
        elif num == 2:
            del_task = input("\nPlease insert the task to remove: ")
            try:
                tasks.remove(del_task)
            except ValueError:
                print("It seems that the task you inserted does not already exist.")
        elif num == 3:
            del_substring = input("\nPlease insert the substring used to match tasks to be removed: ")
            try:
                for del_task in tasks:
                    if del_substring in del_task:
                        tasks.remove(del_task)
            except ValueError:
                print("It seems that the task you inserted does not already exist.")
        elif num == 4:
            print("\nInserted tasks:")
            for task in sorted(tasks):
                print(task)
        elif num == 5:
            with open(sys.argv[1], "w+") as file:
                for task in tasks:
                    file.write("%s\n" % task)
            file.close()
            print("\nExiting...")
        else:
            print("That didn't seem a valid number!")
        print()
    except ValueError:
        print("That didn't seem a number!")
