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
                            echo $_SESSION['userId'] . ", true";
                        } else {
                            echo -1 . ", false";
                        }
                    ?>
                );
                var view = new SeatmapView(model, $(".main-container"));
            }, "JSON");
        });
    </script>

<?php
    require "php/fragments/footer.php"
?>

</body>
</html>