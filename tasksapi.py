# Api for tasks bot
import sqlite3

db = sqlite3.connect("tasks.db")
c = db.cursor()


def close_db():
    db.commit()
    db.close()


def get_tasks():
    query = "SELECT * FROM tasks"
    tasks = []
    for task in c.execute(query).fetchall():
        tasks.append(task[1])
    return sorted(tasks)


def add_task(new_task):
    query = "INSERT INTO tasks(todo) VALUES(?)"
    c.execute(query, (new_task,))
    return True


def delete_task(del_task):
    query = "SELECT COUNT(*) FROM tasks WHERE todo=?"
    res = c.execute(query, (del_task,)).fetchone()
    if res[0] > 0:
        query = "DELETE FROM tasks WHERE todo=?"
        c.execute(query, (del_task,))
        return True
    else:
        return False


def delete_task_match(match_sub):
    query = "SELECT COUNT(*) FROM tasks WHERE todo LIKE ?"
    res = c.execute(query, ("%" + match_sub + "%",)).fetchone()
    if res[0] > 0:
        query = "DELETE FROM tasks WHERE todo LIKE ?"
        c.execute(query, ("%" + match_sub + "%",))
        return True
    else:
        return False
