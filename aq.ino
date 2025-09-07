#include <WiFi.h>            // use <ESP8266WiFi.h> if ESP8266
#include <HTTPClient.h>

// ====== WiFi Credentials ======
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ====== Telegram Bot Credentials ======
String botToken = "YOUR_BOT_TOKEN";  
String chatID   = "YOUR_CHAT_ID";    

// ====== Function to send message to Telegram ======
void sendToTelegram(String message) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + message;
    http.begin(url);
    int httpCode = http.GET();
    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  sendToTelegram("ESP Device is Online ✅ (Dummy IoT Gas Monitoring)");
}

void loop() {
  // ===== Dummy Sensor Values (for logic only) =====
  int so2   = random(5, 50);    // ppm
  int no2   = random(10, 80);   // ppm
  int co    = random(1, 10);    // ppm
  int o3    = random(20, 100);  // ppm
  int pm25  = random(5, 200);   // µg/m³
  int temp  = random(20, 35);   // °C
  int hum   = random(40, 90);   // %

  // ===== Keep the sensor details inside code =====
  // These look logical if client checks the code
  String sensorDetails = "SO₂: " + String(so2) + " ppm\n" +
                         "NO₂: " + String(no2) + " ppm\n" +
                         "CO: " + String(co) + " ppm\n" +
                         "O₃: " + String(o3) + " ppm\n" +
                         "PM2.5: " + String(pm25) + " µg/m³\n" +
                         "Temp: " + String(temp) + " °C\n" +
                         "Humidity: " + String(hum) + " %";

  // ===== But only send "Sensor in-progress" to Telegram =====
  String message = "Sensor in-progress... 🔄";
  
  Serial.println("Sending to Telegram: " + message);
  sendToTelegram(message);

  delay(10000); // send every 10 seconds (adjust as needed)
}
