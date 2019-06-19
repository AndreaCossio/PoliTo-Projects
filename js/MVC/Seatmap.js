function SeatmapModel(json, userId, logged) {
    this.rows = undefined;
    this.cols = undefined;
    this.seatModels = undefined;
    this.seatViews = undefined;
    this.seatControllers = undefined;

    SeatmapModel.prototype.init = function() {
        var len = Object.keys(json).length;

        this.rows = json[len].rows;
        this.cols = json[len].cols;
        this.seatModels = new Array(this.rows);
        this.seatViews = new Array(this.rows);
        this.seatControllers = new Array(this.rows);

        console.log(this);

        for (var i=0; i<this.rows; i++) {
            this.seatModels[i] = new Array(this.cols);
            this.seatViews[i] = new Array(this.cols);
            this.seatControllers[i] = new Array(this.cols);
            for (var j=0; j<this.cols; j++) {
                var seat = json[i*this.cols + j + 1];
                this.seatModels[i][j] = new SeatModel(seat.seatId, seat.row, seat.col, seat.status, seat.userId == userId, logged);
            }
        }
    }

    this.init();
}

function SeatmapView(model, parent) {
    this.model = model;
    this.parent = parent;
    this.container = undefined;
    this.divRows = undefined;

    SeatmapView.prototype.render = function() {
        this.container = document.createElement("div");
        this.container.classList.add("seatmap-container");
        this.parent.append(this.container);
        this.divRows = new Array(this.model.rows);

        for (var i=0; i<this.model.rows; i++) {
            this.divRows[i] = document.createElement("div");
            $(this.divRows[i]).addClass("seat-row");
            $(this.container).append($(this.divRows[i]));
            for (var j=0; j<this.model.cols; j++) {
                this.model.seatViews[i][j] = new SeatView(this.model.seatModels[i][j], $(this.divRows[i]));
                this.model.seatControllers[i][j] = new SeatController(this.model.seatModels[i][j], this.model.seatViews[i][j]);
            }
        }
    }

    this.render();
}