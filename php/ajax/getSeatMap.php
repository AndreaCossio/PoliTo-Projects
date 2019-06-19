<?php

    require_once "../DB.php";

    $seatmap = DatabaseHelper::getInstance()->fetchSeatMap();

    echo json_encode($seatmap);
    exit();