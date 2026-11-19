#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include "server_roots.h"

const int LED_PIN=23;
SemaphoreHandle_t guard;
String mode="OFF", source="BOOT", ack="", outcome="";
bool led=false;
unsigned long serialRevision=0, lastBlink=0;
String input;

// Caller holds guard. Hardware is controlled only by these protected operations.
void applyMode(String next, String origin) {
  mode=next; source=origin; led=(next!="OFF");
  digitalWrite(LED_PIN,led?HIGH:LOW); lastBlink=millis();
  Serial.println("Mode: "+mode+" | source: "+source);
}

void networkTask(void*) {
  WiFi.mode(WIFI_STA); WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  configTime(0,0,"pool.ntp.org","time.google.com");
  for (;;) {
    if (WiFi.status()!=WL_CONNECTED || time(nullptr)<1700000000) {
      Serial.println("Waiting for Wi-Fi / clock. Serial control remains available.");
      vTaskDelay(pdMS_TO_TICKS(5000)); continue;
    }
    JsonDocument report;
    xSemaphoreTake(guard,portMAX_DELAY);
    unsigned long revision=serialRevision;
    report["mode"]=mode; report["source"]=source; report["led"]=led;
    report["ack"]=ack; report["outcome"]=outcome;
    xSemaphoreGive(guard);
    report["uptime"]=millis()/1000UL; report["rssi"]=WiFi.RSSI();
    String body; serializeJson(report,body);
    WiFiClientSecure client; client.setCACert(SERVER_ROOTS); client.setHandshakeTimeout(10);
    HTTPClient http;
    http.setConnectTimeout(10000); http.setTimeout(10000);
    int code=-1;
    if(http.begin(client,String(SERVER_URL)+"/api/device/sync")) {
      http.addHeader("Content-Type","application/json");
      http.addHeader("Authorization",String("Bearer ")+DEVICE_KEY);
      code=http.POST(body);
      if(code==200) {
        JsonDocument response;
        if(!deserializeJson(response,http.getString()) && response["command"].is<JsonObject>()) {
          String id=response["command"]["id"] | "";
          String next=response["command"]["mode"] | "";
          long expires=response["command"]["expires"] | 0L;
          xSemaphoreTake(guard,portMAX_DELAY);
          if(id.length() && id!=ack && (next=="ON"||next=="OFF"||next=="BLINK")) {
            ack=id;
            if(revision!=serialRevision || time(nullptr)>expires) outcome="superseded";
            else { applyMode(next,"WEB"); outcome="applied"; }
          }
          xSemaphoreGive(guard);
        }
      } else Serial.printf("Render sync HTTP %d. Check key/network if persistent.\n",code);
      http.end();
    }
    vTaskDelay(pdMS_TO_TICKS(code==200?2000:10000));
  }
}

void setup() {
  Serial.begin(115200); pinMode(LED_PIN,OUTPUT); digitalWrite(LED_PIN,LOW);
  guard=xSemaphoreCreateMutex();
  if(!guard) { Serial.println("Mutex allocation failed"); while(true) delay(1000); }
  Serial.println("Send ON, OFF, BLINK or STATUS with Newline. GPIO23 LED.");
  if(xTaskCreate(networkTask,"render-sync",16384,nullptr,1,nullptr)!=pdPASS)
    Serial.println("Network task failed; serial control only.");
}
void loop() {
  while(Serial.available()) {
    char c=Serial.read();
    if(c=='\n'||c=='\r') {
      input.trim(); input.toUpperCase();
      if(input.length()) {
        xSemaphoreTake(guard,portMAX_DELAY);
        if(input=="ON"||input=="OFF"||input=="BLINK") { serialRevision++; applyMode(input,"SERIAL"); }
        else if(input=="STATUS") Serial.println("Mode: "+mode+" | source: "+source);
        else Serial.println("Use ON, OFF, BLINK or STATUS.");
        xSemaphoreGive(guard); input="";
      }
    } else if(input.length()<32) input+=c;
  }
  xSemaphoreTake(guard,portMAX_DELAY);
  if(mode=="BLINK" && millis()-lastBlink>=500) {
    led=!led; digitalWrite(LED_PIN,led?HIGH:LOW); lastBlink=millis();
  }
  xSemaphoreGive(guard); delay(2);
}
