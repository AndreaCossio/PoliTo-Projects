# Lab 02 - Exercise 04

# Creating main 2D dictionary
tasks = [{"todo": "call John for AmI project organization", "urgent": True},
         {"todo": "buy a new mouse", "urgent": True},
         {"todo": "find a present for Angelina's birthday", "urgent": False},
         {"todo": "organize mega party (last week of April)", "urgent": False},
         {"todo": "book summer holidays", "urgent": False}, {"todo": "whatsapp Mary for a coffee", "urgent": False}]

# Recipient for urgent tasks
urgent_tasks = []

# Filtering urgent tasks
for task in tasks:
    if task["urgent"]:
        urgent_tasks.append(task)

# Nice printing of tasks
for task in urgent_tasks:
    print(task["todo"])
