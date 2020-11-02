<?php

require_once "functions.php";

class DatabaseHelper {

    const ROWS = 10;
    const COLS = 6;

    private $_connection;
    private static $_instance = null;

    // Creates a new connection
    private function __construct() {
        require_once "Config/db_config.php";
        $this->_connection = new mysqli($default_host, $default_user, $default_pw, $default_db);
        if ($this->_connection->connect_errno) {
            die("Failed to connect to MySQL: " . $this->_connection->connect_error);
        }
    }

    // Closes the connection
    private function __desctruct() {
        $this->_connection->close();
    }

    // Returns the singleton instance
    public static function getInstance() {
        if (!self::$_instance) {
            self::$_instance = new self();
        }
        return self::$_instance;
    }

    // Starts a transaction
    private function startTransaction() {
        $this->_connection->begin_transaction();
    }

    // Commits the transaction
    private function endTransaction() {
        $this->_connection->commit();
    }

    // Rollbacks the transaction
    private function rollback() {
        $this->_connection->rollback();
    }

    // Logs in the user
    public function loginUser($email, $pwd) {
        $result = array();
        $result["success"] = false;

        // Check if the user exists
        try {
            $user = $this->getUser($email);
        } catch (Exception $e) {
            $result["reason"] = "Database failure";
            return $result;
        }
        if ($user->num_rows === 0) {
            // Privacy message: don't tell that the email is not in the database.
            $result["reason"] = "Incorrect password or email.";
            return $result;
        }

        // Fetch user data
        $user_data = $user->fetch_assoc();

        // Verify password
        if (!password_verify($pwd, $user_data['password'])) {
            $result["reason"] = "Incorrect password or email.";
            return $result;
        }
        setSession($user_data['userId'], $user_data['email']);
        return $result;
    }

    // Register the user if it does not already exist
    public function registerUser($email, $pwd) {
        $result = array();
        $result["success"] = false;

        // Validation
        try {
            $this->validate($email, $pwd);
        } catch (Exception $e) {
            $result["reason"] = $e->getMessage();
            return $result;
        }

        // Check if user already exists
        $this->startTransaction();
        try {
            $user = $this->getUserForUpdate($email);
        } catch (Exception $e) {
            $this->rollback();
            $result["reason"] = "Database failure";
            return $result;
        }
        if ($user->num_rows > 0) {
            $this->rollback();
            $result["reason"] = "The user already exists.";
            return $result;
        }

        // Otherwise create it
        try {
            $this->createUser($email, $pwd);
        } catch (Exception $e) {
            $this->rollback();
            $result["reason"] = "Database failure";
            return $result;
        }
        $this->endTransaction();
        
        // Fetch user data to set the session
        try {
            $user_data = $this->getUser($email)->fetch_assoc();
        } catch (Exception $e) {
            $result["reason"] = "Database failure";
            return $result;
        }
        setSession($user_data['userId'], $user_data['email']);
        return $result;
    }

    // Validates the email and checks that the fields are not empty
    private function validate($email, $pwd) {
        // Check empty
        if ($email == "" || $pwd == "") {
            throw new Exception("The email and the password cannot be empty.");
        }
        // Validate email
        if (!preg_match("/^[a-zA-Z0-9\.\!\#\$\%\&\'\*\+\/\=\?\^\_\`\{\|\}\~\-]+@[a-zA-z0-9\-]+\.[a-zA-z0-9]+$/", $email)) {
            throw new Exception("The email is not valid.");
        }
        // Validate pw regex even if it's already validated in the form
        if (!preg_match('/^(?=.*[a-z])(?=.*([A-Z]|[0-9])).*$/', $pwd)) {
            throw new Exception("The password must contain at least one number and one character");
        }
    }

    private function getUser($email) {
        if (!($stmt = $this->_connection->prepare("SELECT * FROM users WHERE email=?;"))) {
            throw new Exception("Prepare failed: (" . $this->_connection->errno . ") " . $this->_connection->error);
        }
        if (!$stmt->bind_param("s", $email)) {
            throw new Exception("Prepare failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        if (!$stmt->execute()) {
            throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        $user = $stmt->get_result();
        $stmt->close();
        return $user;
    }

    private function getUserForUpdate($email) {
        if (!($stmt = $this->_connection->prepare("SELECT * FROM users WHERE email=? FOR UPDATE;"))) {
            throw new Exception("Prepare failed: (" . $this->_connection->errno . ") " . $this->_connection->error);
        }
        if (!$stmt->bind_param("s", $email)) {
            throw new Exception("Prepare failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        if (!$stmt->execute()) {
            throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        $user = $stmt->get_result();
        $stmt->close();
        return $user;
    }

    private function createUser($email, $pwd) {
        if (!($stmt = $this->_connection->prepare("INSERT INTO users (email, password) VALUES (?, ?);"))) {
            throw new Exception("Prepare failed: (" . $this->_connection->errno . ") " . $this->_connection->error);
        }
        $hash = password_hash($pwd, PASSWORD_DEFAULT);
        if (!$stmt->bind_param("ss", $email, $hash)) {
            throw new Exception("Prepare failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        if (!$stmt->execute()) {
            throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        $stmt->close();
        return;
    }

    // Returns the seatmap in an associative array
    public function fetchSeatMap($logged) {
        $rows = self::ROWS;
        $cols = self::COLS;
        $result = array();
        $result["success"] = false;

        try {
            $seatmap = $this->getSeatMap();
        } catch (Exception $e) {
            $result["reason"] = "Database failure";
            return $result;
        }

        $result["success"] = true;
        $result["reason"] = "The seatmap has been correctly updated.";
        $result["data"] = array();

        // Init empty
        for ($i=0; $i<$rows; $i++) {
            for ($j=0; $j<$cols; $j++) {
                $seatId = chr(ord("A") + $j) . ($i + 1);
                if ($logged) {
                    $result["data"][$seatId] = array(
                        "seatId" => $seatId,
                        "mine" => false,
                        "status" => "free"
                    );
                } else {
                    $result["data"][$seatId] = array(
                        "seatId" => $seatId,
                        "status" => "free"
                    );
                }
            }
        }
        
        while ($row = $seatmap->fetch_assoc()) {
            $result["data"][$row["seatId"]]["status"] = $row["status"];
            if ($logged) {
                $result["data"][$row["seatId"]]["mine"] = $row["userId"] == $_SESSION["userId"];
            }
        }

        $result["size"] = array(
            "rows" => $rows,
            "cols" => $cols
        );

        return $result;
    }

    // Reserve the seat for the user if it is not been purchased
    public function reserveSeat($seatId, $userId) {
        $result = array();
        $result["success"] = false;

        $this->startTransaction();

        // Fetch the seat status
        try {
            $query = $this->getSeatForUpdate($seatId);
        } catch (Exception $e) {
            $this->rollback();
            $result["reason"] = "Database failure";
            return $result;
        }

        // If it does not exist
        if ($query->num_rows == 0) {
            try {
                $this->createSeat($seatId, $userId, "reserved");
            } catch (Exception $e) {
                $this->rollback();
                $result["reason"] = "Database failure";
                return $result;
            }
            $result["success"] = true;
            $result["reason"] = "The seat has been correctly reserved.";
        } else {
            // If it is not been purchased in the mean time
            if (($status = $query->fetch_assoc()["status"]) != "purchased") {
                try {
                    $this->updateSeat($seatId, $userId, "reserved");
                } catch (Exception $e) {
                    $this->rollback();
                    $result["reason"] = "Database failure";
                    return $result;
                }
                $result["success"] = true;
                $result["reason"] = "The seat has been correctly reserved.";
            } else {
                $result["reason"] = "Sorry, the seat was purchased in the mean time.";
            }
        }

        $this->endTransaction();

        return $result;
    }

    // Frees a seat
    public function freeSeat($seatId) {
        $result = array();
        $result["success"] = false;

        $this->startTransaction();

        // Fetch the seat status
        try {
            $query = $this->getSeatForUpdate($seatId);
        } catch (Exception $e) {
            $this->rollback();
            $result["reason"] = "Database failure";
            return $result;
        }

        // If it does not exist
        if ($query->num_rows == 0) {
            $result["success"] = true;
            $result["reason"] = "The seat has been correctly freed.";
        } else {
            // If it is not been purchased in the mean time
            if (($status = $query->fetch_assoc()["status"]) != "purchased") {
                try {
                    $this->deleteSeat($seatId);
                } catch (Exception $e) {
                    $this->rollback();
                    $result["reason"] = "Database failure";
                    return $result;
                }
                $result["success"] = true;
                $result["reason"] = "The seat has been correctly freed.";
            } else {
                $result["reason"] = "Sorry, the seat was purchased in the mean time.";
            }
        }

        $this->endTransaction();

        return $result;
    }

    // Purchases the selected seats
    public function purchaseSeats($selected) {
        $result = array();
        $result["success"] = false;
        $canpurchase = true;
        $action = array();

        $this->startTransaction();

        foreach ($selected as $seatId) {
            // Fetch the seat status
            try {
                $query = $this->getSeatForUpdate($seatId);
            } catch (Exception $e) {
                $this->rollback();
                $result["reason"] = "Database failure";
                return $result;
            }

            // If it does not exist
            if ($query->num_rows == 0) {
                $action[$seatId] = "insert";
            } else {
                $seat = $query->fetch_assoc();
                // If not mine or has been purchased in the meantime, remove it from the selected and block purchase
                if ($seat["userId"] != $_SESSION["userId"] || $seat["status"] == "purchased") {
                    $canpurchase = false;
                } else {
                    $action[$seat["seatId"]] = "update";
                }
            }
        }

        foreach ($action as $seatId => $op) {
            try {
                if ($canpurchase) {
                    if ($op == "insert") {
                        $this->createSeat($seatId, $_SESSION["userId"], "purchased");
                    } else {
                        $this->updateSeat($seatId, $_SESSION["userId"], "purchased");
                    }
                } else {
                    if ($op == "update") {
                        $this->deleteSeat($seatId);
                    }
                }
            } catch (Exception $e) {
                $this->rollback();
                $result["reason"] = "Database failure";
                return $result;
            }
        }

        if ($canpurchase) {
            $result["success"] = true;
            $result["reason"] = "The seats have been correctly purchased.";
        } else {
            $result["reason"] = "Sorry, the purchase cannot be placed.";
        }

        $this->endTransaction();
        
        $newseatmap = $this->fetchSeatMap(true);
        if ($newseatmap["success"]) {
            $result["data"] = $newseatmap["data"];
            $result["size"] = $newseatmap["size"];
            return $result;
        } else {
            return $newseatmap;
        }
    }

    private function getSeatMap() {
        if (!($stmt = $this->_connection->prepare("SELECT * FROM seatmap;"))) {
            throw new Exception("Prepare failed: (" . $this->_connection->errno . ") " . $this->_connection->error);
        }
        if (!$stmt->execute()) {
            throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        $seatmap = $stmt->get_result();
        $stmt->close();
        return $seatmap;
    }

    private function getSeatForUpdate($seatId) {
        if (!($stmt = $this->_connection->prepare("SELECT * FROM seatmap WHERE seatId=? FOR UPDATE;"))) {
            throw new Exception("Prepare failed: (" . $this->_connection->errno . ") " . $this->_connection->error);
        }
        if (!$stmt->bind_param("s", $seatId)) {
            throw new Exception("Prepare failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        if (!$stmt->execute()) {
            throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        $seat = $stmt->get_result();
        $stmt->close();
        return $seat;
    }

    private function updateSeat($seatId, $userId, $status) {
        if (!($stmt = $this->_connection->prepare("UPDATE seatmap SET status=?, userId=? WHERE seatId=?;"))) {
            throw new Exception("Prepare failed: (" . $this->_connection->errno . ") " . $this->_connection->error);
        }
        if (!$stmt->bind_param("sis", $status, $userId, $seatId)) {
            throw new Exception("Prepare failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        if (!$stmt->execute()) {
            throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        return;
    }

    private function createSeat($seatId, $userId, $status) {
        if (!($stmt = $this->_connection->prepare("INSERT INTO seatmap VALUES (?, ?, ?);"))) {
            throw new Exception("Prepare failed: (" . $this->_connection->errno . ") " . $this->_connection->error);
        }
        if (!$stmt->bind_param("sis", $seatId, $userId, $status)) {
            throw new Exception("Prepare failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        if (!$stmt->execute()) {
            throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        return;
    }

    private function deleteSeat($seatId) {
        if (!($stmt = $this->_connection->prepare("DELETE FROM seatmap WHERE seatId=?;"))) {
            throw new Exception("Prepare failed: (" . $this->_connection->errno . ") " . $this->_connection->error);
        }
        if (!$stmt->bind_param("s", $seatId)) {
            throw new Exception("Prepare failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        if (!$stmt->execute()) {
            throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
        }
        return;
    }
}

?>