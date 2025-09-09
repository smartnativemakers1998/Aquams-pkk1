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
String botToken = "5379125731:AAHQ9eXgIjZhhdzX3oarlLZS5pt20dRcXpo";  
String chatID   = "-1003003003202";    

// ====== Sensor Pins ======
const int gasSensorPin = 34;   // ESP32 (ADC pin), use A0 for ESP8266
const int dht11Pin     = 27;   // DHT11 signal pin (GPIO27 for ESP32)

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

  // initialize sensor pins
  pinMode(gasSensorPin, INPUT);
  pinMode(dht11Pin, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  sendToTelegram("ESP Device is Online ✅ (IoT Gas + DHT11 Monitoring)");
}

void loop() {
  // ===== Gas Sensor Values =====
  int so2   = random(5, 50);    // ppm
  int no2   = random(10, 80);   // ppm
  int co    = random(1, 10);    // ppm
  int o3    = random(20, 100);  // ppm
  int pm25  = random(5, 200);   // µg/m³

  // ===== DHT11 Values =====
  int temp  = random(20, 35);   // °C
  int hum   = random(40, 90);   // %

  
  int GasSignal = analogRead(gasSensorPin); 
  int DHTSignal = digitalRead(dht11Pin);   

  
  String sensorDetails = "SO₂: " + String(so2) + " ppm\n" +
                         "NO₂: " + String(no2) + " ppm\n" +
                         "CO: " + String(co) + " ppm\n" +
                         "O₃: " + String(o3) + " ppm\n" +
                         "PM2.5: " + String(pm25) + " µg/m³\n" +
                         "Temp: " + String(temp) + " °C\n" +
                         "Humidity: " + String(hum) + " %";

  
  String message = "Sensor in-progress... 🔄";

  Serial.println("Sending to Telegram: " + message);
  Serial.println("Debug Sensor Values:");
  Serial.println(sensorDetails);
  Serial.println("Gas Pin Read: " + String(GasSignal));
  Serial.println("DHT11 Pin Read: " + String(DHTSignal));
  Serial.println("------------------------------------------------");

  sendToTelegram(message);

  delay(10000); // send every 10 seconds
}
