
/*
  ESP32 Health Monitor
  --------------------
  Reads temperature and humidity from DHT11 sensor and sends it to a server via HTTP POST.
  Add your Wi-Fi credentials and server URL before uploading.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// DHT sensor settings
#define DHTPIN 4         // GPIO pin where DHT11 is connected
#define DHTTYPE DHT11    // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi credentials
const char* ssid = "Your_SSID";          // Replace with your Wi-Fi SSID
const char* password = "Your_PASSWORD";  // Replace with your Wi-Fi password

// Server endpoint
const char* serverName = "http://your-server-ip:5000/healthdata"; // Replace with your server address

void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 10) {
    delay(1000);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi");
  } else {
    Serial.println("\nFailed to connect to Wi-Fi");
  }
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(10000);
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // JSON payload
    String json = "{\"temperature\":" + String(temperature) +
                  ",\"humidity\":" + String(humidity) +
                  ",\"systolic\":120,\"diastolic\":80,\"heart_rate\":72}";

    int httpResponseCode = http.POST(json);

    Serial.println("Sending data to server...");
    Serial.println("Response Code: " + String(httpResponseCode));
    http.end();
  } else {
    Serial.println("Wi-Fi not connected. Skipping data send.");
  }

  delay(10000); // Wait 10 seconds before next reading
}
