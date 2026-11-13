// Lesson 2: verify USB upload and serial output. No wiring or credentials needed.
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Skillicon ESP32 USB test started");
}

void loop() {
  Serial.print("ESP32 is running | uptime seconds: ");
  Serial.println(millis() / 1000UL);
  delay(2000);
}
