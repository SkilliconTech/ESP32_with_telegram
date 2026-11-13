#include <WiFi.h>
#include "secrets.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  Serial.println("Connecting to router (30-second initial timeout)...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 30000UL) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected yet. Check credentials and 2.4 GHz Wi-Fi. Retrying periodically.");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi connected | IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(" | Signal (dBm): ");
    Serial.println(WiFi.RSSI());
  } else {
    Serial.println("Wi-Fi disconnected. Retrying...");
    WiFi.reconnect();
  }
  delay(10000); // Fine for this standalone connection test.
}
