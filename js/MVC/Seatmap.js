function SeatmapModel(json, logged) {
    this.json = json;
    this.logged = logged;

    // Initializes variables and creates the seatmodels.
    SeatmapModel.prototype.init = function() {
        this.rows = this.json["size"]["rows"];
        this.cols = this.json["size"]["cols"];
        this.free = 0;
        this.reserved = 0;
        this.purchased = 0;
        this.selected = [];

        this.seatModels = new Array(this.rows);
        for (var i=0; i<this.rows; i++) {
            this.seatModels[i] = new Array(this.cols);
            for (var j=0; j<this.cols; j++) {
                var seat = this.json["data"][String.fromCharCode("A".charCodeAt(0) + j) + (i + 1)];
                this.seatModels[i][j] = new SeatModel(seat, this.logged);
                if (seat["mine"] && seat["status"] == "reserved") {
                    this.selectSeat(seat["seatId"]);
                }
                this.updateCount(seat);
            }
        }
    }

    // Updates all the variables (this is used with the update button)
    // The json is overridden and each seat reference of the json is reassigned to the seatmodel
    SeatmapModel.prototype.updateAll = function(json) {
        this.json = json;
        this.rows = this.json["size"]["rows"];
        this.cols = this.json["size"]["cols"];
        this.free = 0;
        this.reserved = 0;
        this.purchased = 0;
        this.selected = [];

        for (var i=0; i<this.rows; i++) {
            for (var j=0; j<this.cols; j++) {
                var seat = this.json["data"][String.fromCharCode("A".charCodeAt(0) + j) + (i + 1)];
                this.seatModels[i][j].seat = seat;
                if (seat["mine"] && seat["status"] == "reserved") {
                    this.selectSeat(seat["seatId"]);
                }
                this.updateCount(seat);
            }
        }
    }

    // Updates the number of seat for each type according to the new status of the seat
    // This also manages the current reserved seats by the user
    // (This is sed when a single seat is clicked)
    SeatmapModel.prototype.updateSeat = function(seatId, oldStatus) {
        switch (this.json["data"][seatId]["status"]) {
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

    // Updates the count of each seat type accordingly to the seat passed
    // If the seat belongs to the user, the seat is added to the ones selected for purchase
    // (This is used on model updates, not the ones of the seat)
    SeatmapModel.prototype.updateCount = function(seat) {
        switch (seat["status"]) {
            case "free":
                this.free++;
                break;
            case "reserved":
                if (seat["mine"] && this.selected.indexOf(seat["seatId"]) == -1) {
                    this.selected.push(seat["seatId"]);
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

    // Adds the seatId to the selected seats for purchase
    SeatmapModel.prototype.selectSeat = function(seatId) {
        var index = this.selected.indexOf(seatId);
        if (index < 0) {
            this.selected.push(seatId);
        }
    }

    // Removes the seatId from the selected seats for purchase
    SeatmapModel.prototype.unselectSeat = function(seatId) {
        var index = this.selected.indexOf(seatId);
        if (index >= 0) {
            this.selected.splice(index, 1);
        }
    }

    this.init();
}

function SeatmapView(model, parent, freeP, resP, purP) {
    this.model = model;
    this.parent = parent;
    this.freeP = freeP;
    this.resP = resP;
    this.purP = purP;

    // Creates the buttons div and the two buttons if the user is logged
    // Creates the seatmap container
    // Creates rows div and seatViews
    // Updates the three paragraphs that hold the count of seat for each type
    SeatmapView.prototype.init = function() {
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

    // Updates each seat view (is called after a seat model update) and the three paragraphs
    // (This is used with the update button)
    SeatmapView.prototype.updateAll = function() {
        for (var i=0; i<this.model.rows; i++) {
            for (var j=0; j<this.model.cols; j++) {
                this.seatViews[i][j].update();
            }
        }

        // Information
        this.updateNumbers();
    }

    // Updates the three paragraphs according to the model
    SeatmapView.prototype.updateNumbers = function() {
        this.freeP.text(this.model.free);
        this.resP.text(this.model.reserved);
        this.purP.text(this.model.purchased);
    }

    // Set the click listener of the update button
    SeatmapView.prototype.addUpdateClick = function(listener) {
        $(this.updateBtn).off("click");
        $(this.updateBtn).click(listener);
    }

    // Set the click listener of the purchase button
    SeatmapView.prototype.addPurchaseClick = function(listener) {
        $(this.purchaseBtn).off("click");
        $(this.purchaseBtn).click(listener);
    }

    // Removes the click listener of the update button
    // (This is used when the are no seats selected for purchase)
    SeatmapView.prototype.removePurchaseClick = function() {
        $(this.purchaseBtn).off("click");
    }

    this.init();
}

function SeatmapController(model, view) {
    this.model = model;
    this.view = view;

    // Defines the click handlers for the buttons and creates the seat controllers
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

    // Updates the model, the view and reinitializes all the seats controllers
    SeatmapController.prototype.updateAll = function(json) {
        var _this = this;
        this.model.updateAll(json);
        this.view.updateAll();
        for (var i=0; i<_this.model.rows; i++) {
            for (var j=0; j<_this.model.cols; j++) {
                this.seatControllers[i][j].init();
            }
        }
        if (this.model.selected.length > 0) {
            this.view.addPurchaseClick(function() {
                _this.purchase();
            });
        } else {
            this.view.removePurchaseClick();
        }
    }

    // Shows a success toast with the given message
    SeatmapController.prototype.showToastSuccess = function(message) {
        var toast = $(".toast-success");
        var container = $("#toast-success");

        $("#toast-error").css("display", "none");
        container.css("display", "none");

        toast.children(".toast-content").children(".toast-text").text(message);
        toast.children(".toast-close").off("click").on("click", function(e) {
            e.preventDefault();
            container.css("display", "none");
        })
        container.show(300);
    }

    // Shows an error toast with the given message
    SeatmapController.prototype.showToastError = function(message) {
        var toast = $(".toast-error");
        var container = $("#toast-error");

        $("#toast-success").css("display", "none");
        container.css("display", "none");

        toast.children(".toast-content").children(".toast-text").text(message);
        toast.children(".toast-close").off("click").on("click", function(e) {
            e.preventDefault();
            container.css("display", "none");
        })
        container.show(300);
    }

    // Updates the model seat and the numbers of the view
    // This is used when the seat has changed status (reserve or free click)
    SeatmapController.prototype.updateSeat = function(seatId, oldStatus) {
        var _this = this;
        this.model.updateSeat(seatId, oldStatus);
        this.view.updateNumbers();
        if (this.model.selected.length > 0) {
            this.view.addPurchaseClick(function() {
                _this.purchase();
            });
        } else {
            this.view.removePurchaseClick();
        }
    }

    // Handler of the update button
    SeatmapController.prototype.update = function() {
        var _this = this;
        $.post("php/ajax/getSeatmap.php", function(json) {
            if (json["success"]) {
                _this.updateAll(json);
                _this.showToastSuccess(json["reason"]);
            } else {
                if (json["reason"] == "expired") {
                    window.location.href = "../../login.php?error=expired";
                } else {
                    _this.showToastError(json["reason"]);
                }
            }
        }, "JSON");
    }

    // Handler for the purchase button
    SeatmapController.prototype.purchase = function() {
        var _this = this;
        $.post("php/ajax/purchaseSeats.php", {selected: this.model.selected}, function(json) {
            if (json["success"]) {
                _this.updateAll(json);
                _this.showToastSuccess(json["reason"]);
            } else {
                if (json["reason"] != "Database failure") {
                    if (json["reason"] == "expired") {
                        window.location.href = "../../login.php?error=expired";
                    } else {
                        _this.updateAll(json);
                        _this.showToastError(json["reason"]);
                    }
                } else {
                    _this.showToastError(json["reason"]);
                }
            }
        }, "JSON")
    }

    this.init();
}