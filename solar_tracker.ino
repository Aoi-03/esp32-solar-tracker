// ESP32 Solar Tracker with ThingSpeak Integration

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ==== WiFi Credentials ====
const char* ssid = "YourWiFi";
const char* password = "YourPassword";

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

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
  
  sensors.begin();
  tiltServo.attach(13);
  tiltServo.write(pos);
}
WiFiClient client; 
void loop() {
  // === Read Voltage ===
  int rawV = analogRead(VOLTAGE_PIN);
  voltage = (rawV * 3.3 / 4095.0) * 11; // with divider ratio

  // === Read Current ===
  int rawI = analogRead(CURRENT_PIN);
  current = ((rawI * 3.3 / 4095.0) - 2.5) / 0.185; // for ACS712 5A

  // === Power ===
  power = voltage * current;

  // === Temperature ===
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);

  // === LDR values ===
  int l1 = analogRead(LDR1);
  int l2 = analogRead(LDR2);
  int l3 = analogRead(LDR3);
  int l4 = analogRead(LDR4);

  // === Adjust Servo ===
  if ((l1 + l3) > (l2 + l4)) {
    pos += 1;  // Move right
  } else if ((l2 + l4) > (l1 + l3)) {
    pos -= 1;  // Move left
  }
  pos = constrain(pos, 0, 180);
  tiltServo.write(pos);

  // === Print Data ===
  Serial.print("Voltage: "); Serial.print(voltage);
  Serial.print(" V | Current: "); Serial.print(current);
  Serial.print(" A | Power: "); Serial.print(power);
  Serial.print(" W | Temp: "); Serial.print(tempC);
  Serial.print(" C | Servo: "); Serial.println(pos);

  // === Send to ThingSpeak ===
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = serverName + "?api_key=" + apiKey + "&field1=" + String(voltage) +
                 "&field2=" + String(current) + "&field3=" + String(power) +
                 "&field4=" + String(tempC) + "&field5=" + String(pos);
    http.begin(client, url); 
    int httpResponseCode = http.GET();
    http.end();
  }

  delay(10000);
}
