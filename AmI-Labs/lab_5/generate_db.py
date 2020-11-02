import sqlite3

tasks = []

# DB connection and creation of the table
db = sqlite3.connect("tasks.db")
c = db.cursor()
c.execute("DROP TABLE IF EXISTS tasks")
c.execute("CREATE TABLE IF NOT EXISTS tasks(id INTEGER PRIMARY KEY AUTOINCREMENT, todo VARCHAR(256), urgent INTEGER)")

# Reading tasks
try:
    with open("task_list.txt", "r+") as file:
        for line in file:
            tasks.append({"description": line.split(";")[0], "urgent": line.split(";")[1].strip("\n")})
    file.close()
except FileNotFoundError:
    print("FileNotFoundError")

for task in tasks:
    c.execute("INSERT INTO tasks(todo, urgent) VALUES(?,?)", (task["description"], task["urgent"],))

print(c.execute("SELECT * FROM tasks").fetchall())

db.commit()
db.close()
