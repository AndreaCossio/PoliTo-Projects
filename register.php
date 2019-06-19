<?php
    require_once "php/functions.php";
    require_once "php/DB.php";
    
    startSession();

    // HTTPS
    forceHTTPS();

    // Perform registration if coming from the form
    if (isset($_POST['register-submit'])) {
        $error = DatabaseHelper::getInstance()->registerUser($_POST['register-email'], $_POST['register-pwd']);
        if (empty($error))
            redirect("./");
    }

    // Destroy session if the user was logged
    if (isset($_SESSION['userId'])) {
        redirect("./");
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <title>Seat Reserver - Register</title>
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
                <form id="register" action="register.php" method="post">
                    <div class="form-title">
                        <h1>Register</h1>
                        <?php
                            if (isset($error)) {
                                echo "<span>$error</span>";
                            }
                        ?>
                        <hr />
                    </div>
                    <div class="input-container">
                        <label for="register-email"><b>Email</b></label>
                        <span>Your personal email.</span>
                        <input type="email" name="register-email" placeholder="example@email.com" tabindex="1" required>
                    </div>
                    <div class="input-container">
                        <label for="register-pwd"><b>Password</b></label>
                        <span>The password must contain at least 1 lowercase character and 1 uppercase character or number.</span>
                        <input type="password" name="register-pwd" id="register-pwd" placeholder="Password" pattern="^(?=.*[a-z])(?=.*([A-Z]|[0-9])).*$" title="The password must contain at least 1 lowercase character and 1 uppercase character or number." tabindex="2" required>
                    </div>
                    <div class="submit-container">
                        <button type="submit" name="register-submit" class="submitbtn" tabindex="3">Register</button>
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