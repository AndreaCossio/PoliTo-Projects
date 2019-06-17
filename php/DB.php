<?php

require_once "functions.php";

class DatabaseHelper {

    private $_connection;
    private static $_instance = null;

    private function __construct() {
        require_once "Config/db_config.php";
        $this->_connection = new mysqli($default_host, $default_user, $default_pw, $default_db);
        if ($this->_connection->connect_errno) {
            echo "<script>console.log('Failed to connect to MySQL: (" . $this->_connection->connect_errno . ") " . $this->_connection->connect_error . "');</script>";
            exit();
        } else {
            echo "<script>console.log('connected');</script>";
        }
    }

    private function __clone() { }

    public static function getInstance() {
        if (!self::$_instance) {
            self::$_instance = new self();
        }
        return self::$_instance;
    }

    public function registerUser($email, $pwd) {
        if ($email == "" || $pwd == "") {
            return "The email and the password cannot be empty.";
        }
        if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
            return "The email is not valid.";
        }
        $user = $this->selectUser($email);
        if ($user->num_rows > 0) {
            return "The user already exists.";
        }
        $stmt = $this->_connection->prepare("INSERT INTO users (email, password) VALUES (?, ?)");
        $hash = password_hash($pwd, PASSWORD_DEFAULT);
        $stmt->bind_param("ss", $email, $hash);
        $stmt->execute();
        $stmt->close();
        $user_data = $this->selectUser($email)->fetch_assoc();
        $_SESSION['id'] = $user_data['id'];
        $_SESSION['email'] = $user_data['email'];
        redirect("/");
    }

    public function loginUser($email, $pwd) {
        if ($email == "" || $pwd == "") {
            return "The email and the password cannot be empty.";
        }
        if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
            return "The email is not valid.";
        }
        $user = $this->selectUser($email);
        if ($user->num_rows === 0) {
            return "Incorrect password or email.";
        }
        $user_data = $user->fetch_assoc();
        if (password_verify($pwd, $user_data['password'])) {
            $_SESSION['id'] = $user_data['id'];
            $_SESSION['email'] = $user_data['email'];
            redirect("/");
        } else {
            return "Incorrect password or email.";
        }
    }

    public function selectUser($email) {
        $stmt = $this->_connection->prepare("SELECT * FROM users WHERE email=?");
        $stmt->bind_param("s", $email);
        $stmt->execute();
        $user = $stmt->get_result();
        $stmt->close();
        return $user;
    }
}