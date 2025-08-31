// ESP32 Solar Tracker with ThingSpeak Integration
// ESP32 Solar Tracker with ThingSpeak Integration (Optimized Speed)

#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ==== WiFi Credentials ====
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ==== ThingSpeak API ====
String serverName = "https://api.thingspeak.com/update";
String apiKey = "YOUR_API_KEY";

// ==== Pins ====
#define VOLTAGE_PIN 34
#define CURRENT_PIN 35
#define LDR1 32
#define LDR2 33
#define LDR3 25
#define LDR4 26
#define ONE_WIRE_BUS 4
Servo tiltServo;

// ==== Sensors ====
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ==== Variables ====
float voltage, current, power;
float tempC;
int pos = 90; // Servo initial position
unsigned long lastUpdate = 0; // For ThingSpeak timing

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  sensors.begin();
  tiltServo.attach(13); // Servo control pin
  tiltServo.write(pos); // Start in center position
}

void loop() {
  // === Read Voltage ===
  int rawV = analogRead(VOLTAGE_PIN);
  voltage = (rawV * 3.3 / 4095.0) * 11; // Voltage divider ratio

  // === Read Current ===
  int rawI = analogRead(CURRENT_PIN);
  current = ((rawI * 3.3 / 4095.0) - 2.5) / 0.185; // ACS712 formula

  // === Calculate Power ===
  power = voltage * current;

  // === Temperature ===
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);

  // === LDR readings ===
  int l1 = analogRead(LDR1);
  int l2 = analogRead(LDR2);
  int l3 = analogRead(LDR3);
  int l4 = analogRead(LDR4);

  // === Calculate difference ===
  int leftSide = l1 + l3;
  int rightSide = l2 + l4;
  int diff = abs(leftSide - rightSide);

  // === Servo movement (faster & proportional) ===
  if (diff > 50) { // Move only if significant difference
    int step = map(diff, 0, 3000, 1, 15); // Bigger diff → faster move
    if (leftSide > rightSide) {
      pos += step; // Move right
    } else {
      pos -= step; // Move left
    }
    pos = constrain(pos, 0, 180);
    tiltServo.write(pos);
  }

  // === Print Data ===
  Serial.print("Voltage: "); Serial.print(voltage);
  Serial.print(" V | Current: "); Serial.print(current);
  Serial.print(" A | Power: "); Serial.print(power);
  Serial.print(" W | Temp: "); Serial.print(tempC);
  Serial.print(" C | Servo: "); Serial.println(pos);

  // === Send to ThingSpeak every 10 sec ===
  if (millis() - lastUpdate >= 10000) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = serverName + "?api_key=" + apiKey + "&field1=" + String(voltage) +
                   "&field2=" + String(current) + "&field3=" + String(power) +
                   "&field4=" + String(tempC) + "&field5=" + String(pos);
      http.begin(url);
      int httpResponseCode = http.GET();
      Serial.print("ThingSpeak Response: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    lastUpdate = millis();
  }

  delay(200); // Faster servo response
}
