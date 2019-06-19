<?php

    require_once "../DB.php";
    require_once "../functions.php";

    if (sessionExpired()) {
        $result = array();
        $result["success"] = false;
        $result["reason"] = "expired";
        echo json_encode($result);
    } elseif (isset($_POST['selected']) && isset($_SESSION['userId'])) {
        $result = DatabaseHelper::getInstance()->purchaseSeats($_POST['seatId']);
        echo json_encode($result);
    } else {
        redirect("../../");
    }
    
    exit();