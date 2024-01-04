<!DOCTYPE html>
<html>
<head>
<style>
table, th, td {
	border: 1px solid black;
	border-collapse: collapse;
}
</style>
</head>
<body>
<form action="delete_php.php" method="post">
	<input type="submit" value="Delete">
</form>
<br>
<?php

$servername = "***";
$dbname = "***";
$username = "***";
$password = "***";

$conn = new mysqli($servername, $username, $password, $dbname);
if($conn->connect_error) {
	die("Connection failed: " . $conn->connect_error);
}

echo '<table cellspacing="5" cellpadding="5">
			<tr>
				<td>ID</td>
				<td>Trigger Date</td>
				<td>Low Battery</td>
			</tr>';

$sql = "SELECT id, trigger_date, low_battery FROM ESPData ORDER BY id DESC;";

if($result = $conn->query($sql)) {
	while ($row = $result->fetch_assoc()) {
		$row_id = $row["id"];
		$row_trigger_date = $row["trigger_date"];
		$row_low_battery = $row["low_battery"];
		$state = ($row_low_battery == 1) ? "YES" : "NO";

		echo '<tr>
				<td>' . $row_id . '</td>
				<td>' . $row_trigger_date . '</td>
				<td>' . $state . '</td>
			  </tr>';
	}
	$result->free();
}

$conn->close();
echo '</table>'

?>
</body>
</html>
