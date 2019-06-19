function SeatModel(seatId, row, col, status, mine, logged) {
    this.seatId = seatId;
    this.row = row;
    this.col = col;
    this.status = status;
    this.mine = mine;
    this.logged = logged;

    SeatModel.prototype.update = function(mine, status) {
        this.mine = mine;
        this.status = status;
    }
}

function SeatView(model, parent) {
    this.model = model;
    this.parent = parent;
    this.div = undefined;
    this.span = undefined;

    SeatView.prototype.render = function() {
        // Create span and div
        this.span = document.createElement("span");
        this.div = document.createElement("div");
        
        // Set attributes
        $(this.span).html(this.model.row + this.model.col);
        $(this.div).attr('id', this.model.seatId).append(this.span);

        // Update classes and cursor style
        this.update();

        // Inject elements
        $(this.parent).append(this.div);
    }

    SeatView.prototype.update = function() {
        // Update classes
        switch (this.model.status) {
            case "purchased":
                $(this.div).attr("class", "seat purchased");
                break;
            case "reserved":
                if (this.model.mine) {
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
        if (this.model.logged && this.model.status != "purchased") {
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

function SeatController(model, view) {
    this.model = model;
    this.view = view;

    SeatController.prototype.init = function() {
        var _this = this;
        if (this.model.logged && this.model.status != "purchased") {
            if (this.model.status == "reserved" && this.model.mine) {
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

    SeatController.prototype.reserve = function() {
        var _this = this;
        $.post("/php/ajax/reserveSeat.php", {seatId: this.model.seatId}, function(json) {
            if (json.success) {
                _this.model.update(true, "reserved");
                _this.view.update();
                _this.init();
            } else if (!json.success){
                if (json.reason == "expired") {
                    window.location.href = "../../login.php?error=expired";
                } else {
                    _this.model.update(false, "purchased");
                    _this.view.update();
                    _this.init();
                }
            }
        }, "JSON");
    }

    SeatController.prototype.free = function() {
        var _this = this;
        $.post("/php/ajax/freeSeat.php", {seatId: this.model.seatId}, function(json) {
            if (json.success) {
                _this.model.update(false, "free");
                _this.view.update();
                _this.init();
            } else if (!json.success){
                if (json.reason == "expired") {
                    window.location.href = "../../login.php?error=expired";
                } else {
                    _this.model.update(false, "purchased");
                    _this.view.update();
                    _this.init();
                }
            }
        }, "JSON");
    }

    this.init();
}