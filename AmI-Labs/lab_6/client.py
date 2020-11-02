import requests

base_url = 'http://localhost:5000/tasks'


def list_tasks():
    r = requests.get(base_url)
    return r.json()


def task(task_id):
    url = base_url + '/' + str(task_id)
    r = requests.get(url)
    if r.status_code == 200:
        return r.json()
    else:
        return None


def add_task(todo, urgent):
    new_task = {"todo": todo,
                "urgent": urgent}
    r = requests.post(base_url, json=new_task)
    if r.status_code == 201:
        return r.json()
    else:
        return None


def update_task(task_id, todo, urgent):
    upd_task = {"todo": todo,
                "urgent": urgent}
    url = base_url + '/' + str(task_id)
    r = requests.put(url, json=upd_task)
    if r.status_code == 200:
        return r.json()
    else:
        return None


def delete_task(task_id):
    r = requests.delete(base_url + '/' + str(task_id))
    if r.status_code == 200:
        return r.json()
    else:
        return None


if __name__ == '__main__':
    print("Example of the full list got with GET")
    print(list_tasks())

    print("\nExamples of a single task got with GET")
    print(task(1))
    print(task(2))

    print("\nExample of a single task created with POST")
    print(add_task('buy eggs', 0))
    print(list_tasks())

    print("\nExample of a single task updated with PUT")
    print(update_task(2, "buy a new mouse", 0))
    print(list_tasks())

    print("\nExample of a single task deleted with DELETE")
    print(delete_task(3))
    print(list_tasks())
