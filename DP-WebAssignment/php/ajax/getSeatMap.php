<?php

    require_once "../DB.php";
    require_once "../functions.php";

    if (sessionExpired()) {
        $result = array();
        $result["success"] = false;
        $result["reason"] = "expired";
        echo json_encode($result);
    } else {
        if (isset($_SESSION['userId'])) {
            $logged = true;
        } else {
            $logged = false;
        }
        $result = DatabaseHelper::getInstance()->fetchSeatMap($logged);
        echo json_encode($result);
    }
    exit();

?>