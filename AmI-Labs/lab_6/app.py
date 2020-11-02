from flask import Flask, render_template, redirect, url_for, request, jsonify

import tasksapi

app = Flask(__name__)


# HTTP Routes
@app.route('/')
def root():
    return redirect(url_for('index'))


@app.route('/index')
def index():
    tasks = tasksapi.get_tasks()
    return render_template('index.html', tasks=tasks)


@app.route('/deleteTask/<int:task_id>', methods=['GET'])
def delete_task(task_id):
    tasksapi.delete_task(task_id)
    return redirect(url_for('index'))


@app.route('/insertTask', methods=['POST'])
def insert_task():
    if 'urgent' in request.form:
        urgent = True
    else:
        urgent = False
    tasksapi.add_task(request.form['description'], urgent)
    return redirect(url_for('index'))


# API methods
@app.route('/tasks')
def api_tasks():
    return jsonify(tasksapi.get_tasks())


@app.route('/tasks/<int:task_id>')
def api_task(task_id):
    task = [task for task in tasksapi.get_tasks() if task["id"] == task_id]
    if len(task) == 1:
        return jsonify(task)
    else:
        response = jsonify({'message': "Invalid Task Id: " + str(task_id)})
        response.status_code = 404
        return response


@app.route('/tasks', methods=['POST'])
def api_create_task():
    if request.headers['Content-Type'] == 'application/json':
        new_task = request.json
        tasksapi.add_task(new_task["todo"], new_task["urgent"])
        return jsonify({'task': new_task}), 201
    else:
        response = jsonify({'message': "Invalid Request"})
        response.status_code = 404
        return response


@app.route('/tasks/<int:task_id>', methods=['PUT'])
def api_update_task(task_id):
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


@app.route('/tasks/<int:task_id>', methods=['DELETE'])
def api_delete_task(task_id):
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
    tasksapi.close_db()
