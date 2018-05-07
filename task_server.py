from flask import Flask, jsonify, abort, request, Response, render_template
from flask_bootstrap import Bootstrap

import tasksapi

app = Flask(__name__)
Bootstrap(app)


# HTTP Routes
@app.route('/tasks.html')
def tasks():
    return render_template("tasks.html")


# REST Api
@app.route('/api/v1.0/tasks', methods=['GET'])
def get_tasks():
    return jsonify(tasksapi.get_tasks())


@app.route('/api/v1.0/tasks/<int:task_id>', methods=['GET'])
def get_task(task_id):
    task = [task for task in tasksapi.get_tasks() if task["id"] == task_id]
    if len(task) == 1:
        return jsonify(task)
    else:
        response = jsonify({'message': "Invalid Task Id: " + str(task_id)})
        response.status_code = 404
        return response


@app.route('/api/v1.0/tasks', methods=['POST'])
def insert_task():
    if request.headers['Content-Type'] == 'application/json':
        new_task = request.json
        tasksapi.add_task(new_task["todo"], new_task["urgent"])
        return jsonify({'task': new_task}), 201
    else:
        response = jsonify({'message': "Invalid Request"})
        response.status_code = 404
        return response


@app.route('/api/v1.0/tasks/<int:task_id>', methods=['PUT'])
def update_task(task_id):
    if request.headers['Content-Type'] == 'application/json':
        if task_id in [task["id"] for task in tasksapi.get_tasks()]:
            upd_task = request.json
            tasksapi.update_task(task_id, upd_task["todo"], upd_task["urgent"])
            return jsonify([task for task in tasksapi.get_tasks() if task["id"] == task_id][0])
        else:
            response = jsonify({'message': "Invalid Task Id: " + str(task_id)})
            response.status_code = 404
            return response
    else:
        response = jsonify({'message': "Invalid Request"})
        response.status_code = 404
        return response


@app.route('/api/v1.0/tasks/<int:task_id>', methods=['DELETE'])
def delete_task(task_id):
    task = [task for task in tasksapi.get_tasks() if task["id"] == task_id]
    if len(task) != 0:
        if tasksapi.delete_task(task_id):
            return jsonify({'result': True})
        else:
            return jsonify({'result': False})
    else:
        response = jsonify({'message': "Invalid Task Id: " + str(task_id)})
        response.status_code = 404
        return response


if __name__ == '__main__':
    app.run(debug=True)
