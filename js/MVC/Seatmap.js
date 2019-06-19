function SeatmapModel(json, logged) {
    this.logged = logged;

    SeatmapModel.prototype.init = function(json) {
        var len = Object.keys(json).length;
        this.json = json;
        this.rows = json[len].rows;
        this.cols = json[len].cols;
        this.free = 0;
        this.reserved = 0;
        this.purchased = 0;
        this.selected = [];
        this.seatModels = new Array(this.rows);

        for (var i=0; i<this.rows; i++) {
            this.seatModels[i] = new Array(this.cols);
            for (var j=0; j<this.cols; j++) {
                var seat = this.json[i*this.cols + j + 1];
                this.seatModels[i][j] = new SeatModel(seat, this.logged);
                if (seat.mine && status == "reserved") {
                    this.selectSeat(seat.seatId);
                }
                this.updateCount(seat);
            }
        }
    }

    SeatmapModel.prototype.update = function(json) {
        var len = Object.keys(json).length;
        this.json = json;
        this.rows = json[len].rows;
        this.cols = json[len].cols;
        this.free = 0;
        this.reserved = 0;
        this.purchased = 0;
        this.selected = [];
        for (var i=0; i<this.rows; i++) {
            for (var j=0; j<this.cols; j++) {
                var seat = this.json[i*this.cols + j + 1];
                this.seatModels[i][j].seat = seat;
                if (seat.mine && status == "reserved") {
                    this.selectSeat(seat.seatId);
                }
                this.updateCount(seat);
            }
        }
    }

    SeatmapModel.prototype.updateSeat = function(seatId, oldStatus) {
        console.log(seatId);
        switch (this.json[seatId].status) {
            case "free":
                // The user has free it
                this.free++;
                this.reserved--;
                this.unselectSeat(seatId);
                break;
            case "reserved":
                // The user has reserved it
                if (oldStatus == "free") {
                    this.free--;
                    this.reserved++;
                }
                this.selectSeat(seatId);
                break;
            case "purchased":
                // The user selected a cell that was purchased in the meantime
                this.purchased++;
                if (oldStatus == "free") {
                    this.free--;
                } else {
                    this.reserved--;
                }
                break;
            default:
                break;
        }
    }

    SeatmapModel.prototype.updateCount = function(seat) {
        switch (seat.status) {
            case "free":
                this.free++;
                break;
            case "reserved":
                if (seat.mine) {
                    this.selected.push(seat.seatId);
                }
                this.reserved++;
                break;
            case "purchased":
                this.purchased++;
                break;
            default:
                break;
        }
    }

    SeatmapModel.prototype.selectSeat = function(seatId) {
        var index = this.selected.indexOf(seatId);
        if (index < 0) {
            this.selected.push(seatId);
        }
    }

    SeatmapModel.prototype.unselectSeat = function(seatId) {
        var index = this.selected.indexOf(seatId);
        if (index >= 0) {
            this.selected.splice(index, 1);
        }
    }

    this.init(json);
}

function SeatmapView(model, parent, freeP, resP, purP) {
    this.model = model;
    this.parent = parent;
    this.freeP = freeP;
    this.resP = resP;
    this.purP = purP;

    SeatmapView.prototype.render = function() {
        // Create buttons
        if (this.model.logged) {
            var buttons = document.createElement("div");
            buttons.classList.add("buttons");
            this.updateBtn = document.createElement("button");
            this.purchaseBtn = document.createElement("button");

            $(this.updateBtn).text("Update");
            $(this.purchaseBtn).text("Purchase");

            buttons.appendChild(this.updateBtn);
            buttons.appendChild(this.purchaseBtn);

            $(".title-container").append(buttons);
        }

        // Create seatmap container and append it to the parent
        this.container = document.createElement("div");
        this.container.classList.add("seatmap-container");
        this.parent.append(this.container);

        // Create rows and append them to the container
        this.seatRows = new Array(this.model.rows);
        this.seatViews = new Array(this.model.rows);
        for (var i=0; i<this.model.rows; i++) {
            this.seatRows[i] = document.createElement("div");
            this.seatRows[i].classList.add("seat-row");
            this.container.appendChild(this.seatRows[i]);
            this.seatViews[i] = new Array(this.model.cols);
            // Create seats
            for (var j=0; j<this.model.cols; j++) {
                this.seatViews[i][j] = new SeatView(this.model.seatModels[i][j], this.seatRows[i]);
            }
        }

        // Information
        this.updateNumbers();
    }

    SeatmapView.prototype.update = function() {
        for (var i=0; i<this.model.rows; i++) {
            for (var j=0; j<this.model.cols; j++) {
                this.seatViews[i][j].update();
            }
        }

        // Information
        this.updateNumbers();
    }

    SeatmapView.prototype.updateNumbers = function() {
        this.freeP.text(this.model.free);
        this.resP.text(this.model.reserved);
        this.purP.text(this.model.purchased);
    }

    SeatmapView.prototype.addUpdateClick = function(listener) {
        $(this.updateBtn).off("click");
        $(this.updateBtn).click(listener);
    }

    SeatmapView.prototype.addPurchaseClick = function(listener) {
        $(this.purchaseBtn).off("click");
        $(this.purchaseBtn).click(listener);
    }

    SeatmapView.prototype.removePurchaseClick = function() {
        $(this.purchaseBtn).off("click");
    }

    this.render();
}

function SeatmapController(model, view) {
    this.model = model;
    this.view = view;

    SeatmapController.prototype.init = function() {
        var _this = this;
        this.seatControllers = new Array(this.model.rows);
        for (var i=0; i<this.model.rows; i++) {
            this.seatControllers[i] = new Array(this.model.cols);
            for (var j=0; j<this.model.cols; j++) {
                this.seatControllers[i][j] = new SeatController(this.model.seatModels[i][j], this.view.seatViews[i][j], this);
            }
        }
        this.view.addUpdateClick(function() {
            _this.update();
        });
        if (this.model.selected.length > 0) {
            this.view.addPurchaseClick(function() {
                _this.purchase();
            });
        }
    }

    SeatmapController.prototype.update = function() {
        var _this = this;
        $.post("/php/ajax/getSeatMap.php", function(json) {
            _this.model.update(json);
            _this.view.update();
            for (var i=0; i<_this.model.rows; i++) {
                for (var j=0; j<_this.model.cols; j++) {
                    _this.seatControllers[i][j].init();
                }
            }
            if (_this.model.selected.length > 0) {
                _this.view.addPurchaseClick(function() {
                    _this.purchase();
                });
            } else {
                _this.view.removePurchaseClick();
            }
        }, "JSON");
        console.log(this);
    }

    SeatmapController.prototype.updateSeat = function(seatId, oldStatus) {
        this.model.updateSeat(seatId, oldStatus);
        this.view.update();
        console.log(this);
    }

    SeatmapController.prototype.purchase = function() {
        console.log("purchase");
    }

    this.init();
}