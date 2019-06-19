<?php

    require_once "../DB.php";
    require_once "../functions.php";

    startSession();

    if (isset($_SESSION['userId'])) {
        $logged = true;
    } else {
        $logged = false;
    }

    $seatmap = DatabaseHelper::getInstance()->fetchSeatMap($logged);

    echo json_encode($seatmap);
    exit();