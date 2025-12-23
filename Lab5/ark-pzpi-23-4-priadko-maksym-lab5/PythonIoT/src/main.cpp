#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <ArduinoJson.h>

#define BTN_SEARCH 2
#define BTN_CFG 4
#define BUZZER_PIN 15

Preferences prefs;

String deviceId = "esp32-1"; 
String keyword;
bool outputEnabled = true;

String defaultKeyword = "python";
String baseUrl = "https://patents-menu-edges-households.trycloudflare.com";

void beepChar(char c) {
  if (!outputEnabled) return;
  if (c == ' ') { delay(200); return; }
  tone(BUZZER_PIN, 2000);
  delay(80);
  noTone(BUZZER_PIN);
  delay(40);
}

void speakText(const String& text) {
  for (char c : text) beepChar(c);
}

void sendTelemetry(bool success, int resultLength) {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  String url = baseUrl + "/api/iot/log";
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  unsigned long ts = millis();

  String body = "{\"deviceId\":\"" + deviceId + "\","
                "\"keyword\":\"" + keyword + "\","
                "\"success\":" + String(success ? "true" : "false") + ","
                "\"resultLength\":" + String(resultLength) + ","
                "\"timestamp\":" + String(ts) + "}";

  http.POST(body);
  http.end();

  Serial.println("Telemetry sent: " + body);
}

void syncConfig() {
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  String url = baseUrl + "/api/iot/config/" + deviceId;
  http.begin(client, url);

  int code = http.GET();
  if (code == 200) {
    String json = http.getString();
    Serial.println("CONFIG JSON:");
    Serial.println(json);

    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, json);

    if (!err) {
      keyword = doc["keyword"] | defaultKeyword;
      outputEnabled = doc["outputEnabled"] | true;

      prefs.putString("keyword", keyword);
      prefs.putBool("outputEnabled", outputEnabled);

      Serial.print("SYNC keyword: ");
      Serial.println(keyword);
      Serial.print("SYNC outputEnabled: ");
      Serial.println(outputEnabled);
    }
  }

  http.end();
}

void resetConfig() {
  prefs.clear();
  Serial.println("Local config cleared. Restarting...");
  ESP.restart();
}

void connectWifi() {
  WiFi.begin("Wokwi-GUEST", "");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

String extractContent(String json) {
  StaticJsonDocument<4096> doc;
  deserializeJson(doc, json);

  if (doc.containsKey("content")) {
    String result = doc["content"].as<String>();
    result.replace("\\n", " ");
    return result;
  }

  if (doc.containsKey("data") && doc["data"].containsKey("content")) {
    String result = doc["data"]["content"].as<String>();
    result.replace("\\n", " ");
    return result;
  }

  return "";
}

void setup() {
  Serial.begin(115200);

  pinMode(BTN_SEARCH, INPUT_PULLUP);
  pinMode(BTN_CFG, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  connectWifi();

  prefs.begin("iotcfg", false);

  keyword = prefs.getString("keyword", defaultKeyword);
  outputEnabled = prefs.getBool("outputEnabled", true);

  Serial.print("BOOT keyword: ");
  Serial.println(keyword);
  Serial.print("BOOT outputEnabled: ");
  Serial.println(outputEnabled);

  syncConfig();
}

void loop() {
  if (digitalRead(BTN_SEARCH) == LOW) {
    WiFiClientSecure client;
    client.setInsecure();

    String url = baseUrl + "/api/Article/search?keyword=" + keyword;

    HTTPClient http;
    http.begin(client, url);

    int code = http.GET();
    if (code == 200) {
      String json = http.getString();
      String content = extractContent(json);

      Serial.println("ARTICLE:");
      Serial.println(content);

      speakText(content);
      sendTelemetry(true, content.length());

    } else {
      Serial.println("Search failed.");
      sendTelemetry(false, 0);
    }

    http.end();
    delay(800);
  }

  if (digitalRead(BTN_CFG) == LOW) {
    resetConfig();
  }

  static unsigned long last = 0;
  if (millis() - last > 60000) {
    syncConfig();
    last = millis();
  }
}
