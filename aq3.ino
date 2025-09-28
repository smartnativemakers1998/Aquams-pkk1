// ====== Select Board Type ======
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <HTTPClient.h>   // works for both ESP32 and ESP8266

// ====== WiFi Credentials ======
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ====== Telegram Bot Credentials ======
String botToken = "YOUR_BOT_TOKEN";  
String chatID   = "YOUR_CHAT_ID";    

// ====== Sensor Pin (MQ-6) ======
const int mq6Pin = 34;   // Must be an ADC pin on ESP32

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

  // initialize MQ-6 pin
  pinMode(mq6Pin, INPUT);

  // connect WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  sendToTelegram("ESP32 + MQ-6 Device is Online ✅ (Gas Monitoring)");
}

void loop() {
  // ===== Read MQ-6 Sensor =====
  int rawValue = analogRead(mq6Pin);          // 0 - 4095
  float voltage = (rawValue / 4095.0) * 3.3;  // Convert to voltage (ESP32 ADC is 3.3V)

  // Optional: estimate gas level (ppm) → needs calibration
  // For now, we just report raw ADC and voltage
  String message = "MQ-6 Gas Sensor Reading:\n" +
                   String("Raw Value: ") + String(rawValue) + "\n" +
                   String("Voltage: ") + String(voltage, 2) + " V";

  Serial.println(message);
  sendToTelegram(message);

  delay(10000); // send every 10 seconds
}
