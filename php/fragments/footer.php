<script>
    function openNav() {
        document.getElementsByClassName("navbarToggle")[0].classList.toggle("toggled");
        document.getElementById("main-nav").classList.toggle("open");
    }
</script>

<?php
    echo "<script>console.log('Session: " . json_encode($_SESSION) . "');";
    echo "console.log('Cookie: " . json_encode($_COOKIE) . "');";
    echo "console.log('Post: " . json_encode($_POST) . "');";
    echo "console.log('Get: " . json_encode($_GET) . "');";
    echo "console.log('Env: " . json_encode($_ENV) . "');";
    echo "console.log('Files: " . json_encode($_FILES) . "');";
    echo "console.log('Request: " . json_encode($_REQUEST) . "');";
    echo "console.log('Globals: " . json_encode($GLOBALS) . "');";
    echo "console.log('URI: " . $_SERVER['REQUEST_URI'] . "');";
    echo "console.log('ScriptName: " . $_SERVER['SCRIPT_NAME'] . "');</script>";
?>