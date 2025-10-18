// ESP_forwarder.ino
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "HOGWART";
const char* password = "dracarys!!";

const char* serverHost = "192.168.0.111";
const int serverPort   = 80;
const char* addSensorPath = "/electro_dashboard/add_data.php";
const char* addCountPath  = "/electro_dashboard/add_counts.php";

void setup() {
  Serial.begin(115200);
  delay(200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("ESP IP: "); 
  Serial.println(WiFi.localIP());
}

void forwardToServer(String line) {
  line.trim();
  if (line.length() == 0) return;

  Serial.print("Received: ");
  Serial.println(line);

  if (line.startsWith("COUNT,")) {
    // --- Human counts line ---
    int idx1 = line.indexOf(',', 6);
    int idx2 = line.indexOf(',', idx1+1);
    if (idx1 > 0 && idx2 > idx1) {
      long total   = line.substring(6, idx1).toInt();
      long escC4   = line.substring(idx1+1, idx2).toInt();
      long escC5   = line.substring(idx2+1).toInt();

      // Debug print extracted values
      Serial.printf("Parsed Counts -> Total: %ld | C4: %ld | C5: %ld\n", total, escC4, escC5);

      String url = String("http://") + serverHost + ":" + serverPort + addCountPath +
        "?total=" + total +
        "&c4=" + escC4 +
        "&c5=" + escC5;

      Serial.print("Sending Counts URL: ");
      Serial.println(url);

      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, url);
        int httpCode = http.GET();
        if (httpCode > 0) {
          String payload = http.getString();
          Serial.printf("HTTP %d: %s\n", httpCode, payload.c_str());
        } else {
          Serial.printf("HTTP failed: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      }
    }
  } else {
    // --- Sensor line ---
    int idx1 = line.indexOf(',');
    int idx2 = line.indexOf(',', idx1+1);
    int idx3 = line.indexOf(',', idx2+1);
    int idx4 = line.indexOf(',', idx3+1);

    if (idx1 > 0 && idx2 > idx1 && idx3 > idx2 && idx4 > idx3) {
      int corridor = line.substring(0, idx1).toInt();
      int flame    = line.substring(idx1+1, idx2).toInt();
      int gas      = line.substring(idx2+1, idx3).toInt();
      float temp   = line.substring(idx3+1, idx4).toFloat();
      float hum    = line.substring(idx4+1).toFloat();

      Serial.printf("Parsed Sensor -> Corridor: %d | Flame: %d | Gas: %d | Temp: %.1f | Hum: %.1f\n", corridor, flame, gas, temp, hum);

      String url = String("http://") + serverHost + ":" + serverPort + addSensorPath +
        "?corridor=" + corridor +
        "&flame=" + flame +
        "&gas=" + gas +
        "&temp=" + String(temp, 1) +
        "&hum=" + String(hum, 1);

      Serial.print("Sending Sensor URL: ");
      Serial.println(url);

      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, url);
        int httpCode = http.GET();
        if (httpCode > 0) {
          String payload = http.getString();
          Serial.printf("HTTP %d: %s\n", httpCode, payload.c_str());
        } else {
          Serial.printf("HTTP failed: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      }
    } else {
      Serial.println("Malformed CSV received.");
    }
  }
}

String serialBuffer = "";

void loop() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      serialBuffer.trim();
      if (serialBuffer.length() > 0) {
        forwardToServer(serialBuffer);
      }
      serialBuffer = "";
    } else {
      serialBuffer += c;
    }
  }
}
