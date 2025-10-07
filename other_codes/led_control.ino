#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ====== Wi-Fi credentials ======
const char* ssid = "HOGWART";
const char* password = "dracarys!!";

// ====== LED pin ======
#define LED_PIN D1  // GPIO5

ESP8266WebServer server(80);  // Web server runs on port 80

bool ledState = LOW;

// ====== HTML page ======
String htmlPage() {
  String stateText = (ledState) ? "ON" : "OFF";
  String buttonColor = (ledState) ? "red" : "green";
  String buttonText = (ledState) ? "Turn OFF" : "Turn ON";

  String html = "<!DOCTYPE html><html><head><title>ESP8266 LED Control</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;background:#f2f2f2;}";
  html += "h2{color:#333;}";
  html += "button{padding:15px 25px;font-size:18px;border:none;border-radius:8px;cursor:pointer;color:white;background:" + buttonColor + ";}";
  html += "</style></head><body>";
  html += "<h2>ESP8266 LED Control</h2>";
  html += "<p>LED is currently <strong>" + stateText + "</strong></p>";
  html += "<form action='/toggle'><button type='submit'>" + buttonText + "</button></form>";
  html += "</body></html>";
  return html;
}

// ====== Handle root (main page) ======
void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

// ====== Handle toggle button ======
void handleToggle() {
  ledState = !ledState;          // change state
  digitalWrite(LED_PIN, ledState);
  server.send(200, "text/html", htmlPage());
}

// ====== Setup ======
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected!");
  Serial.print("ESP8266 IP Address: ");
  Serial.println(WiFi.localIP());

  // setup web routes
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
  Serial.println("HTTP server started!");
}

// ====== Loop ======
void loop() {
  server.handleClient();  // handle web requests
}
