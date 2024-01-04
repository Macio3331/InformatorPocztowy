<?php

$servername = "***";
$dbname = "***";
$username = "***";
$password = "***";
$api_key_value = "***";

$api_key = $trigger_date = $trigger_time = $low_battery = "";

if($_SERVER["REQUEST_METHOD"] == "POST") {
	$api_key = test_input($_POST["api_key"]);
	if($api_key == $api_key_value) {
		$trigger_time = test_input($_POST["trigger_time"]);
		$low_battery = test_input($_POST["low_battery"]);

		$trigger_date = date("Y-m-d") . " " . $trigger_time;

		$conn = new mysqli($servername, $username, $password, $dbname);
		if($conn->connect_error) {
			die("Connection failed: " . $conn->connect_error);
		}

		$sql = "INSERT INTO ESPData (trigger_date, low_battery) VALUES ('" . $trigger_date . "', '" . $low_battery . "');";
		if($conn->query($sql) == TRUE) {
			echo "New record created successfully!";
		}
		else {
			echo "Error: " . $sql . "<br>" . $conn->error;
		}

		$conn->close();
	}
	else {
		echo "Wrong API Key provided!";
	}
}
else {
	echo "No data posted with HTTP POST.";
}

function test_input($data) {
	$data = trim($data);
	$data = stripslashes($data);
	$data = htmlspecialchars($data);
	return $data;
}
