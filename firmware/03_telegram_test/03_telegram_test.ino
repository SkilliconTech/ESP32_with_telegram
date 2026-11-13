#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <time.h>
#include "secrets.h"

WiFiClientSecure telegramClient;
UniversalTelegramBot bot(BOT_TOKEN, telegramClient);
bool ready = false;
unsigned long lastPoll = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 30000UL) {
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi failed. Check secrets.h and reset to retry.");
    return;
  }
  Serial.println("Wi-Fi connected. Synchronizing clock for HTTPS...");
  configTime(0, 0, "pool.ntp.org", "time.google.com");
  started = millis();
  while (time(nullptr) < 1700000000 && millis() - started < 30000UL) {
    delay(250);
  }
  if (time(nullptr) < 1700000000) {
    Serial.println("Time sync failed. Check internet/NTP access and reset.");
    return;
  }
  telegramClient.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  bot.waitForResponse = 10000;
  if (!bot.getMe()) {
    Serial.println("Telegram connection failed. Check token, internet and library certificate. Reset to retry.");
    return;
  }
  ready = true;
  Serial.println("Telegram connected. Send /start in your bot's private chat.");
  if (String(ALLOWED_CHAT_ID).length() == 0) {
    Serial.println("ID discovery: no replies until you configure ALLOWED_CHAT_ID.");
  }
}

void loop() {
  if (!ready || WiFi.status() != WL_CONNECTED) {
    delay(100);
    return;
  }
  if (millis() - lastPoll < 2000UL) return;
  int count = bot.getUpdates(bot.last_message_received + 1);
  for (int i = 0; i < count; i++) {
    // A private chat's chat ID matches the sender's user ID.
    if (bot.messages[i].chat_id != bot.messages[i].from_id) continue;
    if (String(ALLOWED_CHAT_ID).length() == 0) {
      Serial.print("Private chat ID: ");
      Serial.println(bot.messages[i].chat_id);
      continue;
    }
    if (bot.messages[i].chat_id != String(ALLOWED_CHAT_ID)) continue;
    String reply;
    if (bot.messages[i].text == "/start") {
      reply = "Hello from your ESP32-WROOM-32! Telegram communication is working.";
    } else if (bot.messages[i].text == "/status") {
      reply = "ESP32 online | uptime seconds: " + String(millis() / 1000UL);
    } else {
      reply = "Available now: /start, /help, /status. LED control and OTA are not implemented yet.";
    }
    Serial.println(bot.sendMessage(bot.messages[i].chat_id, reply, "")
      ? "Telegram reply sent." : "Reply failed. Send the command again to retry.");
  }
  lastPoll = millis();
}
