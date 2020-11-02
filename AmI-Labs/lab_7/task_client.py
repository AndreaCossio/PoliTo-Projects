import requests
import json


def print_tasks_from_server():
    resp = requests.get('http://127.0.0.1:5000/api/v1.0/tasks')
    if resp.status_code != 200:
        print("Error: the list of tasks is not available at this moment.")
    else:
        for task in json.loads(resp.text)['tasks']:
            print(task['description'] + " " +str(task['urgent']))


if __name__ == '__main__':
    print_tasks_from_server()