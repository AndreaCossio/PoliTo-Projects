<?php
function startSession() {
    if (!isset($_SESSION)) {
        session_start();
    }
}

function destroySession() {
    $_SESSION = array();
    if (ini_get("session.use_cookies")) {
        $params = session_get_cookie_params();
        setcookie(session_name(), '', time() - 3600*24, $params["path"], $params["domain"], $params["secure"], $params["httponly"]);
    }
    session_destroy();
}

function logout() {
    startSession();
    if (isset($_SESSION['id'])) {
        destroySession();
    }
}

function forceHTTPS() {
    if (!isset($_SERVER['HTTPS']) || $_SERVER['HTTPS'] != "on") {
        redirect("https://" . $_SERVER["HTTP_HOST"] . $_SERVER["REQUEST_URI"]);
        exit();
    }
}

function requireCookies() {
    setcookie("cookies", "true");
    if (!(isset($_COOKIE['cookies']) && $_COOKIE['cookies'] = "true")) {
        echo "<style>.main-container{display:none;}.no-cookies{display:block;}</style>";
    }
}

function redirect($url) {
    header("Location: " . $url, true, 303);
    exit();
}
?>