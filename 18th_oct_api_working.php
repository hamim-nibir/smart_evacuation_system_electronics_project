<?php
// api.php
header('Content-Type: application/json; charset=utf-8');

// Database settings
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

$max_corridors = 5;

// Get latest row for each corridor
$sql = "
SELECT t.corridor_id, t.flame, t.gas, t.temperature, t.humidity, t.recorded_at
FROM sensor_data t
INNER JOIN (
  SELECT corridor_id, MAX(recorded_at) AS mx
  FROM sensor_data
  WHERE corridor_id BETWEEN 1 AND ?
  GROUP BY corridor_id
) latest 
ON latest.corridor_id = t.corridor_id AND latest.mx = t.recorded_at
ORDER BY t.corridor_id ASC
";

$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $max_corridors);
$stmt->execute();
$res = $stmt->get_result();

$result = [];
$risks = [];

// Initialize all corridors + exits with default cost
for ($i=1; $i <= $max_corridors; $i++) {
    $result[$i] = null;
    $risks[$i] = 10; // default medium risk
}

// Process rows from DB
while ($row = $res->fetch_assoc()) {
    $cid = intval($row['corridor_id']);
    $flame = intval($row['flame']);
    $gas = intval($row['gas']);
    $temperature = floatval($row['temperature']);
    $humidity = floatval($row['humidity']);
    $recorded_at = $row['recorded_at'];

    // Risk classification (improved)
    $risk = 'low';
    $risk_cost = 5;

    if ($flame < 400) {
        $risk = 'high';
        $risk_cost = 20;
    } else if ($flame < 900) {
        $risk = 'medium';
        $risk_cost = 10;
    }

    $risks[$cid] = $risk_cost;

    $result[$cid] = [
        'flame' => $flame,
        'gas' => $gas,
        'temperature' => $temperature,
        'humidity' => $humidity,
        'recorded_at' => $recorded_at,
        'risk' => $risk
    ];
}

$stmt->close();
$conn->close();

// -------- PATHFINDING --------

// Graph structure (corridors + exits as nodes)
$graph = [
  1 => [2],
  2 => [1,3,4],
  3 => [2,5],
  4 => [2], // Exit 2
  5 => [3], // Exit 1
];
$exits = [4,5];

function dijkstra($graph, $risks, $start, $exits) {
    $dist = [];
    $prev = [];
    $Q = [];

    foreach ($graph as $node => $edges) {
        $dist[$node] = INF;
        $prev[$node] = null;
        $Q[$node] = true;
    }
    $dist[$start] = 0;

    while (!empty($Q)) {
        // Get node with min distance
        $u = null;
        foreach ($Q as $node => $_) {
            if ($u === null || $dist[$node] < $dist[$u]) {
                $u = $node;
            }
        }
        if ($u === null) break;
        unset($Q[$u]);

        if (in_array($u, $exits)) break;

        // Relax neighbors
        foreach ($graph[$u] as $v) {
            if (!isset($Q[$v])) continue;
            $alt = $dist[$u] + ($risks[$v] ?? 10);
            if ($alt < $dist[$v]) {
                $dist[$v] = $alt;
                $prev[$v] = $u;
            }
        }
    }

    // Find nearest exit
    $nearestExit = null;
    $minDist = INF;
    foreach ($exits as $e) {
        if ($dist[$e] < $minDist) {
            $minDist = $dist[$e];
            $nearestExit = $e;
        }
    }

    // Reconstruct path
    $path = [];
    if ($nearestExit !== null) {
        for ($at=$nearestExit; $at!==null; $at=$prev[$at]) {
            array_unshift($path, $at);
        }
    }

    return [
      "from" => $start,
      "exit" => $nearestExit,
      "cost" => $minDist,
      "path" => $path
    ];
}

// Compute safest path for corridors 1–3 (not exits)
$paths = [];
foreach ([1,2,3] as $c) {
    $paths[$c] = dijkstra($graph, $risks, $c, $exits);
}

// Final response
$response = [
    "corridors" => $result,
    "paths" => $paths
];

echo json_encode($response, JSON_PRETTY_PRINT);
