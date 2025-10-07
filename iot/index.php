<?php
// index.php
?>
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Corridor Dashboard — Real-time Monitor</title>
<link rel="stylesheet" href="style.css">
</head>
<body>
<div class="container">
<header>
<div>
<h1>Corridor Monitoring Dashboard</h1>
<!-- <div class="subtitle">5 corridors · real-time readings from ESP8266 via PHP/MySQL</div> -->
</div>
<div class="muted">Updated: <span id="lastUpdate">--</span></div>
</header>

<div class="topcards">
<div class="stat card">
<div class="muted">Corridors monitored</div>
<div class="big" id="totalCorridors">5</div>
</div>
<div class="stat card">
<div class="muted">High risk right now</div>
<div class="big" id="highCount">0</div>
</div>
<div class="stat card">
<div class="muted">Medium risk</div>
<div class="big" id="medCount">0</div>
</div>
</div>

<div class="grid" id="cards">
<!-- Cards inserted by JS -->
</div>
</div>

<script>
const apiUrl = "api.php"; // endpoint
const pollInterval = 3000; // ms

function loadData() {
    fetch("fetch_data.php")
        .then(res => res.text())
        .then(html => {
            document.getElementById("sensor-table").innerHTML = html;
        });
}

// load immediately
loadData();
// refresh every 3 seconds
setInterval(loadData, 3000);

// Generate 5 corridor cards
function makeCards() {
    const container = document.getElementById("cards");
    for (let i = 1; i <= 5; i++) {
        container.innerHTML += `
        <div class="card" id="corridor-${i}">
          <h2>Corridor ${i}</h2>
          <div class="meta">
            <div class="id">#${i}</div>
            <div class="time" id="time-${i}">--</div>
          </div>
          <div class="values">
            <div class="value"><div class="label">Flame</div><div class="num" id="flame-${i}">--</div><div class="barwrap"><div class="bar" id="bar-flame-${i}"></div></div></div>
            <div class="value"><div class="label">Gas</div><div class="num" id="gas-${i}">--</div><div class="barwrap"><div class="bar" id="bar-gas-${i}"></div></div></div>
            <div class="value"><div class="label">Temperature</div><div class="num" id="temp-${i}">--</div></div>
            <div class="value"><div class="label">Humidity</div><div class="num" id="hum-${i}">--</div></div>
          </div>
          <div class="risk">
            <span>Risk</span>
            <span class="badge low" id="risk-${i}">--</span>
          </div>
        </div>`;
    }
}

function formatTime(str) {
    if (!str) return "--";
    return new Date(str).toLocaleTimeString();
}

function updateUI(data) {
    let high = 0, med = 0;
    for (let i = 1; i <= 5; i++) {
        const c = data[i] || {};
        document.getElementById("flame-"+i).textContent = c.flame ?? "--";
        document.getElementById("gas-"+i).textContent = c.gas ?? "--";
        document.getElementById("temp-"+i).textContent = c.temperature ?? "--";
        document.getElementById("hum-"+i).textContent = c.humidity ?? "--";
        document.getElementById("time-"+i).textContent = formatTime(c.recorded_at);

        // progress bars
        document.getElementById("bar-flame-"+i).style.width = c.flame ? Math.min(100, (c.flame/1023*100))+"%" : "0%";
        document.getElementById("bar-gas-"+i).style.width = c.gas ? Math.min(100, (c.gas/1023*100))+"%" : "0%";

        // risk badge
        const riskEl = document.getElementById("risk-"+i);
        if (c.risk === "high") {
            riskEl.textContent = "HIGH";
            riskEl.className = "badge high";
            high++;
        } else if (c.risk === "medium") {
            riskEl.textContent = "MEDIUM";
            riskEl.className = "badge medium";
            med++;
        } else if (c.risk === "low") {
            riskEl.textContent = "LOW";
            riskEl.className = "badge low";
        } else {
            riskEl.textContent = "--";
            riskEl.className = "badge low";
        }
    }
    document.getElementById("lastUpdate").textContent = new Date().toLocaleTimeString();
    document.getElementById("highCount").textContent = high;
    document.getElementById("medCount").textContent = med;
}

async function poll() {
    try {
        const res = await fetch(apiUrl + "?t=" + Date.now());
        if (!res.ok) throw new Error("Network");
        const data = await res.json();
        updateUI(data);
    } catch (e) {
        console.error("poll error", e);
    } finally {
        setTimeout(poll, pollInterval);
    }
}

// init
makeCards();
poll();
</script>
</body>
</html>
