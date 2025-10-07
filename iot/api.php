<?php
// api.php
header('Content-Type: application/json; charset=utf-8');

// DB settings (match add_data.php)
$db_host = 'localhost';
$db_user = 'root';
$db_pass = '';
$db_name = 'corridor_db';

$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);
if ($conn->connect_error) {
    http_response_code(500);
    echo json_encode(['error'=>'DB connection failed']);
    exit;
}

// corridors count
$max_corridors = 5;

// We'll get the latest row for each corridor using a subquery
$sql = "
SELECT t.corridor_id, t.flame, t.gas, t.temperature, t.humidity, t.recorded_at
FROM sensor_data t
INNER JOIN (
  SELECT corridor_id, MAX(recorded_at) AS mx
  FROM sensor_data
  WHERE corridor_id BETWEEN 1 AND ?
  GROUP BY corridor_id
) latest ON latest.corridor_id = t.corridor_id AND latest.mx = t.recorded_at
ORDER BY t.corridor_id ASC
";

$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $max_corridors);
$stmt->execute();
$res = $stmt->get_result();

$result = [];
// Initialize with empty object for each corridor index so UI doesn't break
for ($i=1; $i <= $max_corridors; $i++) {
    $result[$i] = null;
}

while ($row = $res->fetch_assoc()) {
    $cid = intval($row['corridor_id']);
    $flame = isset($row['flame']) ? intval($row['flame']) : null;
    $gas = isset($row['gas']) ? intval($row['gas']) : null;
    $temperature = isset($row['temperature']) ? floatval($row['temperature']) : null;
    $humidity = isset($row['humidity']) ? floatval($row['humidity']) : null;
    $recorded_at = $row['recorded_at'];

    // Simple risk logic: tweak thresholds to your needs
    $risk = 'low';
    if (($flame !== null && $flame > 800) || ($gas !== null && $gas > 800)) $risk = 'high';
    else if (($flame !== null && $flame > 400) || ($gas !== null && $gas > 400)) $risk = 'medium';

    $result[$cid] = [
        'flame' => $flame,
        'gas' => $gas,
        'temperature' => $temperature,
        'humidity' => $humidity,
        'recorded_at' => $recorded_at,
        'risk' => $risk
    ];
}

echo json_encode($result);
$stmt->close();
$conn->close();
?>
