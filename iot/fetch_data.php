<?php
$pdo = new PDO("mysql:host=localhost;dbname=your_db;charset=utf8mb4", "db_user", "db_pass");

$stmt = $pdo->query("SELECT * FROM sensor_data ORDER BY created_at DESC LIMIT 10");
$rows = $stmt->fetchAll(PDO::FETCH_ASSOC);

echo "<table border='1' cellpadding='5'>";
echo "<tr><th>Time</th><th>Corridor</th><th>Flame</th><th>Gas</th><th>Temperature</th><th>Humidity</th></tr>";
foreach ($rows as $r) {
    echo "<tr>";
    echo "<td>" . htmlspecialchars($r['created_at']) . "</td>";
    echo "<td>" . htmlspecialchars($r['corridor']) . "</td>";
    echo "<td>" . htmlspecialchars($r['flame']) . "</td>";
    echo "<td>" . htmlspecialchars($r['gas']) . "</td>";
    echo "<td>" . htmlspecialchars($r['temperature']) . "</td>";
    echo "<td>" . htmlspecialchars($r['humidity']) . "</td>";
    echo "</tr>";
}
echo "</table>";
?>
