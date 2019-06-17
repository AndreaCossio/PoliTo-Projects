<?php
    require_once "php/functions.php";
    
    startSession();

    if (isset($_SESSION['id'])) {
        // Force HTTPS
        forceHTTPS();

        // Grab the time
        $now = time();
        $inactivity = 120; // 2 minutes

        // Manage inactivity
        if (isset($_SESSION['last_action'])) {
            if (($now - $_SESSION['last_action']) > $inactivity) {
                destroySession();
                redirect("login.php?error=expired");
            }
        }
        $_SESSION['last_action'] = $now;
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
            <span>Hey</span>
        </div>
        <?php require_once "php/fragments/js-cookies.php"?>
    </div>

<?php
    require "php/fragments/footer.php"
?>

</body>
</html>