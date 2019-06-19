function SeatModel(seat, logged) {
    this.seat = seat;
    this.logged = logged;

    SeatModel.prototype.update = function(mine, status) {
        this.seat.mine = mine;
        this.seat.status = status;
    }
}

function SeatView(model, parent) {
    this.model = model;
    this.parent = parent;

    SeatView.prototype.render = function() {
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

    SeatView.prototype.addClick = function(listener) {
        $(this.div).off("click");
        $(this.div).click(listener);
    }

    SeatView.prototype.removeClick = function() {
        $(this.div).off("click");
    }

    this.render();
}

function SeatController(model, view, parentController) {
    this.model = model;
    this.view = view;

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

    SeatController.prototype.update = function(mine, status, oldStatus) {
        this.model.update(mine, status);
        this.view.update();
        this.init();
        parentController.updateSeat(this.model.seat.seatId, oldStatus);
    }

    SeatController.prototype.reserve = function() {
        var _this = this;
        var oldStatus = this.model.seat.status;
        $.post("/php/ajax/reserveSeat.php", {seatId: this.model.seat.seatId}, function(json) {
            if (json.success) {
                _this.update(true, "reserved", oldStatus);
            } else if (!json.success){
                if (json.reason == "expired") {
                    window.location.href = "../../login.php?error=expired";
                } else {
                    _this.update(false, "purchased");
                }
            }
        }, "JSON");
    }

    SeatController.prototype.free = function() {
        var _this = this;
        var oldStatus = this.model.seat.status;
        $.post("/php/ajax/freeSeat.php", {seatId: this.model.seat.seatId}, function(json) {
            if (json.success) {
                _this.update(false, "free", oldStatus);
            } else if (!json.success){
                if (json.reason == "expired", oldStatus) {
                    window.location.href = "../../login.php?error=expired";
                } else {
                    _this.update(false, "purchased", oldStatus);
                }
            }
        }, "JSON");
    }

    this.init();
}