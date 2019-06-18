<aside id="main-aside" class="nav-container">
    <nav class="navbar">
    <?php
        if (strpos($_SERVER['SCRIPT_NAME'], "index")) {
            if (isset($_SESSION['id'])) {
                echo '  <a href="javascript:void(0)" class="active">Home</a>
                        <a href="logout.php">Logout</a>';
            } else {
                echo '  <a href="javascript:void(0)" class="active">Home</a>
                        <a href="login.php">Login</a>
                        <a href="register.php">Register</a>';
            }
        } elseif (strpos($_SERVER['SCRIPT_NAME'], "login")) {
            echo '  <a href="/">Home</a>
                    <a href="javascript:void(0)" class="active">Login</a>
                    <a href="register.php">Register</a>';
        } elseif (strpos($_SERVER['SCRIPT_NAME'], "register")) {
            echo '  <a href="/">Home</a>
                    <a href="login.php">Login</a>
                    <a href="javascript:void(0)" class="active">Register</a>';
        }
    ?>
    </nav>
</aside>