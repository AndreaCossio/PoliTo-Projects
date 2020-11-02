from flask import Flask, render_template, redirect, url_for, request
import tasksapi

app = Flask(__name__)


@app.route('/')
def root():
    return redirect(url_for('index'))


@app.route('/index')
def index():
    tasks = tasksapi.get_tasks()
    print(tasks)
    return render_template('index.html', tasks=tasks)


@app.route('/deleteTask/<int:id>', methods=['GET'])
def delete_task(id):
    tasksapi.delete_task(id)
    return redirect(url_for('index'))


@app.route('/insertTask', methods=['POST'])
def insert_task():
    if 'urgent' in request.form:
        urgent = 1
    else:
        urgent = 0
    tasksapi.add_task(request.form['description'], urgent)
    return redirect(url_for('index'))


if __name__ == '__main__':
    app.run(debug=True)
    tasksapi.close_db()
