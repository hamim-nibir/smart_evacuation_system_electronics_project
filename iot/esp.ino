// ESP_forwarder.ino
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "HOGWART";
const char* password = "dracarys!!";

// your PHP server host (ip or domain) and path
const char* serverHost = "192.168.0.105"; // replace with your server IP or domain
const int serverPort = 80;
const char* addPath = "/electro_dashboard/add_data.php"; // FIXED ✅

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("WiFi connect failed - continuing (will retry).");
  }
}

String urlEncode(const String &str) {
  String encoded = "";
  char c;
  char hex[] = "0123456789ABCDEF";
  for (int i = 0; i < str.length(); i++) {
    c = str[i];
    if ( (c >= '0' && c <= '9') ||
         (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else {
      encoded += '%';
      encoded += hex[(c >> 4) & 0xF];
      encoded += hex[c & 0xF];
    }
  }
  return encoded;
}

void loop() {
  // Reconnect WiFi if lost
  if (WiFi.status() != WL_CONNECTED) {
    static unsigned long lastTry = 0;
    if (millis() - lastTry > 5000) {
      Serial.println("Reconnecting WiFi...");
      WiFi.begin(ssid, password);
      lastTry = millis();
    }
  }

  // Read line from serial (Mega)
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) return;
    Serial.print("Received from Mega: ");
    Serial.println(line);

    // expected: corridor,flame,gas,temp,hum
    // Example: 1,234,512,28.6,61.2
    int corridor=0, flame=0, gas=0;
    float temp=0, hum=0;

    // parse
    int idx1 = line.indexOf(',');
    int idx2 = line.indexOf(',', idx1+1);
    int idx3 = line.indexOf(',', idx2+1);
    int idx4 = line.indexOf(',', idx3+1);

    if (idx1 > 0 && idx2 > idx1 && idx3 > idx2 && idx4 > idx3) {
      String s_corr = line.substring(0, idx1);
      String s_flame = line.substring(idx1+1, idx2);
      String s_gas = line.substring(idx2+1, idx3);
      String s_temp = line.substring(idx3+1, idx4);
      String s_hum  = line.substring(idx4+1);

      corridor = s_corr.toInt();
      flame = s_flame.toInt();
      gas = s_gas.toInt();
      temp = s_temp.toFloat();
      hum  = s_hum.toFloat();

      // Build full URL
      String url = String("http://") + serverHost + ":" + serverPort + addPath +
        "?corridor=" + String(corridor) +
        "&flame=" + String(flame) +
        "&gas=" + String(gas) +
        "&temp=" + String(temp, 1) +
        "&hum=" + String(hum, 1);

      // Send to server
      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, url);   // ✅ fixed API
        int httpCode = http.GET();
        if (httpCode > 0) {
          String payload = http.getString();
          Serial.printf("HTTP %d: %s\n", httpCode, payload.c_str());
        } else {
          Serial.printf("HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      } else {
        Serial.println("WiFi not connected; cannot forward data.");
      }
    } else {
      Serial.println("Malformed line from Mega - expected CSV.");
    }
  }

  delay(10);
}
