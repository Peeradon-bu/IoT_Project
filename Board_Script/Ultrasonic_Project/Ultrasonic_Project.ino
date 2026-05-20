#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// --- ตั้งค่า WiFi ---
const char* ssid = "Don";
const char* password = "12345678";

// --- ตั้งค่า Supabase ---
const char* supabase_url = "https://vtjkgfausdyldboiaein.supabase.co/rest/v1/smart_bin";
const char* supabase_key = "sb_publishable_5jYp51pYwD1ulw5ktU9vPA_-wa6d3ST";

// --- ตั้งค่าขา Pin Ultrasonic ---
const int trigPin = 5;
const int echoPin = 18;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  long duration;
  float distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  sendToSupabase(distance);

  delay(30000); // รอ 1 นาทีก่อนวัดใหม่
}

void sendToSupabase(float dist) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(supabase_url);
    http.addHeader("apikey", supabase_key);
    http.addHeader("Authorization", "Bearer " + String(supabase_key));
    http.addHeader("Content-Type", "application/json");

    String isFullValue = (dist < 10) ? "true" : "false";
    String jsonPayload = "{\"distance\": " + String(dist, 2) + ", \"is_full\": " + isFullValue + "}";
    
    int httpResponseCode = http.POST(jsonPayload);
    Serial.println("Supabase Response: " + String(httpResponseCode));
    http.end();
  }
}