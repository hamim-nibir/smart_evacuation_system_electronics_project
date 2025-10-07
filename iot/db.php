<?php
// add_data.php
// Example usage (GET):
// http://your-server/add_data.php?corridor=1&flame=100&gas=200&temp=28.5&hum=60

// --- DB settings - change these to match your server ---
$db_host = 'localhost';
$db_user = 'root';
$db_pass = '';
$db_name = 'corridor_db';

// connect
$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);
if ($conn->connect_error) {
    http_response_code(500);
    echo json_encode(['status'=>'error','msg'=>'DB connection failed']);
    exit;
}

$corridor = isset($_REQUEST['corridor']) ? intval($_REQUEST['corridor']) : 0;
$flame = isset($_REQUEST['flame']) ? intval($_REQUEST['flame']) : null;
$gas = isset($_REQUEST['gas']) ? intval($_REQUEST['gas']) : null;
$temp = isset($_REQUEST['temp']) ? floatval($_REQUEST['temp']) : null;
$hum = isset($_REQUEST['hum']) ? floatval($_REQUEST['hum']) : null;

if ($corridor <= 0) {
    http_response_code(400);
    echo json_encode(['status'=>'error','msg'=>'corridor param required']);
    $conn->close();
    exit;
}

// prepared insert
$stmt = $conn->prepare("INSERT INTO sensor_data (corridor_id, flame, gas, temperature, humidity) VALUES (?, ?, ?, ?, ?)");
$stmt->bind_param("iiidd", $corridor, $flame, $gas, $temp, $hum);

if ($stmt->execute()) {
    echo json_encode(['status'=>'ok', 'insert_id' => $stmt->insert_id]);
} else {
    http_response_code(500);
    echo json_encode(['status'=>'error','msg'=>$stmt->error]);
}

$stmt->close();
$conn->close();
?>
