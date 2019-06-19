<?php
    require_once "php/functions.php";

    if (sessionExpired()) {
        redirect("./login.php?error=expired");
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <title>Seat Reserver - Homepage</title>
    <?php require_once "php/fragments/head.php";?>
</head>
<body>

    <?php
        require_once "php/fragments/header.php";
        require_once "php/fragments/navbar.php";
    ?>

    <div class="main">
        <div class="main-container">
            <div class="title-container">
                <div class="places">
                    <h1>Total places: <span>60</span></h1>
                    <p>Free: <span>0</span></p>
                    <p>Reserved: <span>0</span></p>
                    <p>Purchased: <span>0</span></p>
                </div>
            </div>
        </div>
        <?php require_once "php/fragments/js-cookies.php"?>
    </div>

    <script type="text/javascript" src="/js/MVC/Seat.js"></script>
    <script type="text/javascript" src="/js/MVC/Seatmap.js"></script>
    <script type="text/javascript">
        $(document).ready(function() {
            $.post("/php/ajax/getSeatMap.php", function(json) {
                var model = new SeatmapModel(json,
                    <?php
                        if (isset($_SESSION['userId'])) {
                            echo "true";
                        } else {
                            echo "false";
                        }
                    ?>
                );
                var view = new SeatmapView(model, $(".main-container"), $($(".places p")[0]).children("span"), $($(".places p")[1]).children("span"), $($(".places p")[2]).children("span"));
                var controller = new SeatmapController(model, view);
            }, "JSON");
        });
    </script>

<?php
    require "php/fragments/footer.php"
?>

</body>
</html>