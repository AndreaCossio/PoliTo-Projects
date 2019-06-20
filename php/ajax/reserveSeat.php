<?php

    require_once "../DB.php";
    require_once "../functions.php";

    if (sessionExpired()) {
        $result = array();
        $result["success"] = false;
        $result["reason"] = "expired";
        echo json_encode($result);
    } elseif (isset($_POST['seatId']) && isset($_SESSION['userId'])) {
        $result = DatabaseHelper::getInstance()->reserveSeat((int)$_POST['seatId'], $_SESSION['userId']);
        echo json_encode($result);
    } else {
        redirect("../../");
    }

    exit();

?>