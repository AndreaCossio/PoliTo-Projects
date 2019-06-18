<?php

require_once "functions.php";

class DatabaseHelper {

    private $_connection;
    private static $_instance = null;

    private function __construct() {
        require_once "Config/db_config.php";
        $this->_connection = new mysqli($default_host, $default_user, $default_pw, $default_db);
        if ($this->_connection->connect_errno) {
            die("Failed to connect to MySQL: " . $this->_connection->connect_error);
        }
    }

    private function __clone() { }

    private function __desctruct() {
        $this->_connection->close();
    }

    public static function getInstance() {
        if (!self::$_instance) {
            self::$_instance = new self();
        }
        return self::$_instance;
    }

    private function startTransaction() {
        $this->_connection->begin_transaction();
    }

    private function endTransaction() {
        $this->_connection->commit();
    }

    private function rollback() {
        $this->_connection->rollback();
    }

    public function loginUser($email, $pwd) {
        // Validation
        try {
            $this->validate($email, $pwd);
        } catch (Exception $e) {
            return $e->getMessage();
        }

        // Check if the user exists
        try {
            $user = $this->getUser($email);
        } catch (Exception $e) {
            die($e->getMessage());
        }
        if ($user->num_rows === 0) {
            // Privacy message: don't tell that the email is not in the database.
            return "Incorrect password or email.";
        }

        // Fetch user data
        $user_data = $user->fetch_assoc();

        // Verify password
        if (password_verify($pwd, $user_data['password'])) {
            setSession($user_data['id'], $user_data['email']);
            redirect("/");
        } else {
            return "Incorrect password or email.";
        }
    }

    public function registerUser($email, $pwd) {
        // Validation
        try {
            $this->validate($email, $pwd);
        } catch (Exception $e) {
            return $e->getMessage();
        }

        // Validate pw regex even if it's already validated in the form
        if (!preg_match('/^(?=.*[a-z])(?=.*([A-Z]|[0-9])).*$/', $pwd)) {
            throw new Exception("The password must contain at least one number and one character");
        }

        // Check if user already exists
        $this->startTransaction();
        try {
            $user = $this->getUserForUpdate($email);
        } catch (Exception $e) {
            $this->rollback();
            die($e->getMessage());
        }
        if ($user->num_rows > 0) {
            $this->rollback();
            return "The user already exists.";
        }

        // Otherwise create it
        try {
            $this->createUser($email, $pwd);
        } catch (Exception $e) {
            $this->rollback();
            die($e->getMessage());
        }
        $this->endTransaction();
        
        // Fetch user data to set the session
        try {
            $user_data = $this->getUser($email)->fetch_assoc();
        } catch (Exception $e) {
            die($e->getMessage());
        }
        setSession($user_data['id'], $user_data['email']);
        redirect("/");
    }

    private function validate($email, $pwd) {
        if ($email == "" || $pwd == "") {
            throw new Exception("The email and the password cannot be empty.");
        }
        if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
            throw new Exception("The email is not valid.");
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
}