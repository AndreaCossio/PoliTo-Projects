# Lab 02 - Exercise 04

# Creating main 2D dictionary
tasks = {"task1": {"todo": "call John for AmI project organization", "urgent": True},
         "task2": {"todo": "buy a new mouse", "urgent": True},
         "task3": {"todo": "find a present for Angelina's birthday", "urgent": False},
         "task4": {"todo": "organize mega party (last week of April)", "urgent": False},
         "task5": {"todo": "book summer holidays", "urgent": False},
         "task6": {"todo": "whatsapp Mary for a coffee", "urgent": False}}

# Recipient for urgent tasks
urgent_tasks = {}

# Filtering urgent tasks
for task in tasks:
    if tasks[task]["urgent"]:
        urgent_tasks[task] = tasks[task]

# Nice printing of tasks
print("Urgent tasks:")
for task in urgent_tasks:
    print(urgent_tasks[task]["todo"])
