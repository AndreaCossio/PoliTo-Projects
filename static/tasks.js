let RESTAPI = "http://127.0.0.1:5000/api/v1.0";

$(document).ready(function () {
    resetFields();
    generateList();
});

function generateList() {
    $("#tasklist").html("<ul class='list-group'></ul>");

    $.getJSON(RESTAPI + "/tasks", function(data){
        for(let index in data) {
            let finalStringToAppend = "<li class='list-group-item ";
            if(data[index].urgent == 1)
                finalStringToAppend += "list-group-item-danger'>";
            else
                finalStringToAppend += "list-group-item-info'>";
            finalStringToAppend += data[index].todo + "<button onclick='deleteTask(this)' type='button' class='btn btn-link' data-task-id='" + data[index].id + "'>Delete</button>";
            finalStringToAppend += "<button onclick='updateHandler(this)' type='button' class='btn btn-link' data-task-id='" + data[index].id + "'>Update</button></li>";
            $("#tasklist ul").append($(finalStringToAppend));
        }
    });
}

function updateHandler(identifier) {
    let taskId = $(identifier).data('task-id');
    $.getJSON(RESTAPI + "/tasks/" + taskId, function(data){
        updateFields(data[0].todo, data[0].urgent, taskId)
    });
}

function updateFields(description, urgent, taskId) {
    $("#taskDescription").val(description);
    $("#urgentTask").prop("checked", urgent == 1 ? true : false);
    $("#addTask").html("Update");
    $("#addForm").unbind().attr("method", "put").submit(function() {
        let todo = $("#taskDescription").val();
        let urgent = $("#urgentTask").is(":checked");
        let task = JSON.stringify({"todo": todo, "urgent": urgent ? 1 : 0});

        updateTask(task, taskId);

        return false;
    });
}

function resetFields() {
    $("#taskDescription").val("");
    $("#urgentTask").prop("checked", false);
    $("#addTask").html("Add");
    $("#addForm").unbind().attr("method", "get").submit(function() {
        let todo = $("#taskDescription").val();
        let urgent = $("#urgentTask").is(":checked");
        let task = JSON.stringify({"todo": todo, "urgent": urgent ? 1 : 0});

        addTask(task);

        return false;
    });
}

function addTask(task){
    $.post({
        "url": RESTAPI + "/tasks",
        "data": task,
        "contentType": "application/json",
        "success": function() {generateList()}
    });
}

function deleteTask(identifier) {
    let taskId = $(identifier).data('task-id');
    $.ajax({
        "url": RESTAPI + "/tasks/" + taskId,
        "type": 'DELETE',
        "success": function () {generateList()}
    });
}

function updateTask(task, taskId) {
    $.ajax({
       "url": RESTAPI + "/tasks/" + taskId,
       "type": 'PUT',
       "data": task,
       "contentType": "application/json",
       "success": function () {
           generateList();
           resetFields();
       }
    });
}