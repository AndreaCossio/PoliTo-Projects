# Lab 02 - Exercise 03

tasks = []
taskfile = "task_list.txt"


def read_tasks():
    try:
        with open(taskfile, "r+") as file:
            for line in file:
                tasks.append(line.strip("\n"))
        file.close()
        return True
    except FileNotFoundError:
        return False


def write_tasks():
    try:
        with open(taskfile, "w+") as file:
            for task in tasks:
                file.write("%s\n" % task)
        file.close()
        return True
    except FileNotFoundError:
        return False


def get_tasks():
    return sorted(tasks)


def add_task(new_task):
    if new_task in tasks:
        return False
    else:
        tasks.append(new_task)
        return True


def delete_task(del_task):
    try:
        tasks.remove(del_task)
        return True
    except ValueError:
        return False


def delete_task_match(match_sub):
    try:
        for task in tasks:
            if match_sub in task:
                tasks.remove(task)
        return True
    except ValueError:
        return False
