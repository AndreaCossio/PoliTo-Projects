<?php
function startSession() {
    if (!isset($_SESSION)) {
        session_start();
    }
}

function setSession($id, $email) {
    startSession();
    $_SESSION['userId'] = $id;
    $_SESSION['email'] = $email;
}

function sessionExpired() {
    startSession();
    if (isset($_SESSION['userId'])) {
        // Force HTTPS
        forceHTTPS();

        // Grab the time
        $now = time();
        $inactivity = 120; // 2 minutes

        // Manage inactivity
        if (isset($_SESSION['last_action'])) {
            if (($now - $_SESSION['last_action']) > $inactivity) {
                destroySession();
                return true;
            }
        }
        $_SESSION['last_action'] = $now;
    }
    return false;
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
    if (isset($_SESSION['userId'])) {
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
        echo "<style type='text/css'>.main-container{display:none;}.no-cookies{display:block;}</style>";
    }
}

function redirect($url) {
    header("Location: ". $url, true, 303);
    exit();
}
?>