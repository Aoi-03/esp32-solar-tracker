# esp32-solar-tracker
ESP32 Solar Tracker. This project is an IoT-based solar tracker using an ESP32 microcontroller. It automatically adjusts the position of a solar panel using four LDRs and a servo motor to maximize sunlight exposure. The system also measures voltage, current, power, and temperature, and uploads this data to ThingSpeak for remote monitoring.
Features

Automatic Sun Tracking: Uses four LDRs and a servo to follow the sun.
Real-Time Monitoring: Measures voltage, current, power, and temperature.
IoT Data Logging: Sends sensor data to ThingSpeak for visualization and analysis.
Easy Customization: Simple code structure for adding more sensors or features.
Hardware Required

ESP32 Dev Board
4 × LDRs (Light Dependent Resistors)
1 × Servo Motor
1 × Dallas DS18B20 Temperature Sensor
1 × ACS712 Current Sensor
Voltage divider circuit for voltage measurement
Jumper wires, breadboard, and solar panel
Libraries Used

WiFi.h
HTTPClient.h
Servo.h
OneWire.h
DallasTemperature.h
Getting Started

Clone this repository:
git clone [https://github.com/yourusername/esp32-solar-tracker.git](https://github.com/Aoi-03/esp32-solar-tracker/tree/main)
Open solar_tracker.ino in Arduino IDE.
Install required libraries via Arduino Library Manager.
Update your WiFi credentials and ThingSpeak API key in the code:
const char* ssid = "YourWiFi";
const char* password = "YourPassword";
String apiKey = "YOUR_API_KEY";
Upload the code to your ESP32 board.
Connect the hardware as per the pin definitions in the code.
Pin Connections

| Function         | ESP32 Pin |
|------------------|-----------|
| Voltage Sensor   | 34        |
| Current Sensor   | 35        |
| LDR1             | 32        |
| LDR2             | 33        |
| LDR3             | 25        |
| LDR4             | 26        |
| Temp Sensor      | 4         |
| Servo            | 13        |
