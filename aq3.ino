// ====== Select Board Type ======
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

#include <HTTPClient.h>   // works for both ESP32 and ESP8266
#include "DHT.h"          // DHT11 Library

// ====== WiFi Credentials ======
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ====== Telegram Bot Credentials ======
String botToken = "YOUR_BOT_TOKEN";  
String chatID   = "YOUR_CHAT_ID";    

// ====== Sensor Pins ======
const int mq6Pin = 34;    // MQ-6 analog output (ADC pin on ESP32)
const int dhtPin = 27;    // DHT11 signal pin

// ====== DHT Configuration ======
#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

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

  // initialize sensors
  pinMode(mq6Pin, INPUT);
  dht.begin();

  // connect WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  sendToTelegram("ESP32 Device Online ✅ (MQ-6 + DHT11 Monitoring)");
}

void loop() {
  // ===== Read MQ-6 Sensor =====
  int rawValue = analogRead(mq6Pin);          // 0 - 4095
  float voltage = (rawValue / 4095.0) * 3.3;  // Convert to voltage

  // ===== Read DHT11 Sensor =====
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  // Handle possible read errors
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return; // skip this loop
  }

  // ===== Prepare message =====
  String message = "📡 Sensor Readings:\n";
  message += "MQ-6 Gas Raw: " + String(rawValue) + "\n";
  message += "MQ-6 Voltage: " + String(voltage, 2) + " V\n";
  message += "Temp: " + String(temp, 1) + " °C\n";
  message += "Humidity: " + String(hum, 1) + " %";

  // ===== Debug to Serial =====
  Serial.println(message);
  Serial.println("---------------------------");

  // ===== Send to Telegram =====
  sendToTelegram(message);

  delay(10000); // send every 10 seconds
}
