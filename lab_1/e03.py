# Lab 01 - Exercise 03

# List of tasks
tasks = []
num = -1

# Main loop
while num != 4:

    # Printing menu
    print("""Insert the number corresponding to the action you want to perform:
            1. Insert a new task
            2. Remove a task (by typing its content exactly)
            3. Show all existing tasks
            4. Close the program""")

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
            print("\nInserted tasks:")
            for task in tasks:
                print(task)
        elif num == 4:
            print("\nExiting...")
        else:
            print("That didn't seem a valid number!")
        print()
    except ValueError:
        print("That didn't seem a number!")
