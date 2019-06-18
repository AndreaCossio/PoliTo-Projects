<?php
    require_once "php/functions.php";
    require_once "php/DB.php";
    
    startSession();
    
    // HTTPS
    forceHTTPS();

    // Perform login if coming from the form
    if (isset($_POST['login-submit'])) {
        $error = DatabaseHelper::getInstance()->loginUser($_POST['login-email'], $_POST['login-pwd']);
    }

    // Destroy session if the user was logged
    if (isset($_SESSION['id'])) {
        redirect("/");
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <title>Seat Reserver - Login</title>
    <?php require_once "php/fragments/head.php";?>
</head>
<body>

    <?php
        require_once "php/fragments/header.php";
        require_once "php/fragments/navbar.php";
    ?>

    <div class="main">
        <div class="main-container">
            <div class="form-container">
                <form id="login" action="login.php" method="post">
                    <div class="form-title">
                        <h1>Login</h1>
                        <?php
                            if (isset($_GET['error'])) {
                                echo "<span>The session has expired, please re-authenticate.</span>";
                            } elseif (isset($error)) {
                                echo "<span>$error</span>";
                            }
                        ?>
                        <hr />
                    </div>
                    <div class="input-container">
                        <label for="login-email"><b>Email</b></label>
                        <span>Your personal email.</span>
                        <input type="email" name="login-email" placeholder="example@email.com" tabindex="1" required>
                    </div>
                    <div class="input-container">
                        <label for="login-pwd"><b>Password</b></label>
                        <span>Your personal password.</span>
                        <input type="password" name="login-pwd" placeholder="Password" tabindex="2" required>
                    </div>
                    <div class="submit-container">
                        <button type="submit" name="login-submit" class="submitbtn" tabindex="3">Login</button>
                    </div>
                </form>
            </div>
        </div>
        <?php require_once "php/fragments/js-cookies.php"?>
    </div>

    <?php
        require "php/fragments/footer.php"
    ?>

</body>
</html>