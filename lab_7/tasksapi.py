import sqlite3

db = sqlite3.connect("tasks.db", check_same_thread=False)
c = db.cursor()


def get_tasks():
    query = "SELECT * FROM tasks"
    tasks = []
    for task in c.execute(query).fetchall():
        if task[2] == 1:
            tasks.append({"id": task[0], "todo": task[1], "urgent": True})
        else:
            tasks.append({"id": task[0], "todo": task[1], "urgent": False})
    return tasks


def add_task(new_task, urgent):
    query = "INSERT INTO tasks(todo, urgent) VALUES(?,?)"
    if urgent:
        c.execute(query, (new_task, 1))
    else:
        c.execute(query, (new_task, 0))
    db.commit()
    return True


def update_task(task_id, upd_task, urgent):
    print(task_id, upd_task, urgent)
    query = "UPDATE tasks SET todo=?, urgent=? WHERE id=?"
    if urgent:
        c.execute(query, (upd_task, 1, task_id))
    else:
        c.execute(query, (upd_task, 0, task_id))
    db.commit()
    return True


def delete_task(del_task):
    query = "SELECT COUNT(*) FROM tasks WHERE id=?"
    res = c.execute(query, (del_task,)).fetchone()
    if res[0] > 0:
        query = "DELETE FROM tasks WHERE id=?"
        c.execute(query, (del_task,))
        db.commit()
        return True
    else:
        db.commit()
        return False


def delete_task_match(match_sub):
    query = "SELECT COUNT(*) FROM tasks WHERE todo LIKE ?"
    res = c.execute(query, ("%" + match_sub + "%",)).fetchone()
    if res[0] > 0:
        query = "DELETE FROM tasks WHERE todo LIKE ?"
        c.execute(query, ("%" + match_sub + "%",))
        db.commit()
        return True
    else:
        db.commit()
        return False
