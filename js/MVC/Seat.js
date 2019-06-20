function SeatModel(seat, logged) {
    this.seat = seat;
    this.logged = logged;

    // Updates the seat. This update will change also the parent model's json
    // because the seat is referenced.
    SeatModel.prototype.update = function(mine, status) {
        this.seat.mine = mine;
        this.seat.status = status;
    }
}

function SeatView(model, parent) {
    this.model = model;
    this.parent = parent;

    // Creates seat div and span and set classes and cursor
    // Then appends them to the parent
    SeatView.prototype.init = function() {
        // Create span and div
        this.span = document.createElement("span");
        this.div = document.createElement("div");
        
        // Set seat text and append it
        $(this.span).html(this.model.seat.row + this.model.seat.col);
        $(this.div).append(this.span);

        // Update classes and cursor style
        this.update();

        // Apeend div
        $(this.parent).append(this.div);
    }

    // Updates the seat background and cursor
    // according to the model
    SeatView.prototype.update = function() {
        // Update classes
        switch (this.model.seat.status) {
            case "purchased":
                $(this.div).attr("class", "seat purchased");
                break;
            case "reserved":
                if (this.model.seat.mine) {
                    $(this.div).attr("class", "seat reserved-me");
                } else {
                    $(this.div).attr("class", "seat reserved");
                }
                break;
            case "free":
                $(this.div).attr("class", "seat free");
                break;
            default:
                break;
        }

        // Update cursor
        if (this.model.logged && this.model.seat.status != "purchased") {
            $(this.div).css("cursor", "pointer");
            $(this.div).children("span").css("cursor", "pointer");
        } else {
            $(this.div).css("cursor", "default");
            $(this.div).children("span").css("cursor", "default");
        }
    }

    // Set the click listener of the div
    SeatView.prototype.addClick = function(listener) {
        $(this.div).off("click");
        $(this.div).click(listener);
    }

    // Remove the click listener of the div
    SeatView.prototype.removeClick = function() {
        $(this.div).off("click");
    }

    this.init();
}

function SeatController(model, view, parentController) {
    this.model = model;
    this.view = view;
    this.parentController = parentController;

    // Defines the click handler for the seat according to the model
    SeatController.prototype.init = function() {
        var _this = this;
        if (this.model.logged && this.model.seat.status != "purchased") {
            if (this.model.seat.status == "reserved" && this.model.seat.mine) {
                this.view.addClick(function() {
                    _this.free();
                })
            } else {
                this.view.addClick(function() {
                    _this.reserve();
                });
            }
        } else {
            this.view.removeClick();
        }
    }

    // Updates the model and the view of the seat and resets the click handler
    // Notifies also the parent controller to update the parent model and view
    SeatController.prototype.update = function(mine, status, oldStatus) {
        this.model.update(mine, status);
        this.view.update();
        this.init();
        this.parentController.updateSeat(this.model.seat.seatId, oldStatus);
    }

    // Handler for a reserve click
    SeatController.prototype.reserve = function() {
        var _this = this;
        $.post("/php/ajax/reserveSeat.php", {seatId: this.model.seat.seatId}, function(json) {
            var old = _this.model.seat.status;
            if (json.success) {
                _this.update(true, "reserved", old);
                _this.parentController.showToastSuccess("The seat has been correctly reserved.");
            } else if (json.reason != "failure") {
                if (json.reason == "expired") {
                    window.location.href = "../../login.php?error=expired";
                } else {
                    _this.update(false, "purchased", old);
                    _this.parentController.showToastError("Sorry, the seat was reserved in the mean time.");
                }
            }
            //TODO ERROR AND TOAST NOTIFICATION
        }, "JSON");
    }

    // Handler for a free click
    SeatController.prototype.free = function() {
        var _this = this;
        $.post("/php/ajax/freeSeat.php", {seatId: this.model.seat.seatId}, function(json) {
            var old = _this.model.seat.status;
            if (json.success) {
                _this.update(false, "free", old);
                _this.parentController.showToastSuccess("The seat has been correctly freed.");
            } else if (json.reason != "failure") {
                if (json.reason == "expired") {
                    window.location.href = "../../login.php?error=expired";
                } else {
                    _this.update(false, "purchased", old);
                    _this.parentController.showToastError("Sorry, the seat was reserved in the mean time.");
                }
            }
            //TODO ERROR AND TOAST NOTIFICATION
        }, "JSON");
    }

    this.init();
}