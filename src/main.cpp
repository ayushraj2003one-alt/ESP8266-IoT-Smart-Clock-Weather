/**
 * @file main.cpp / ESP8266_Clock_Weather.ino
 * @brief ESP8266 IoT Digital Clock & Environmental Weather Monitor
 * @details Features local Access Point (SoftAP), dynamic web dashboard for time/date adjustment,
 *          OLED display, DS1307 RTC, and DHT11 climate sensing with non-blocking timing.
 * @author Ayush Raj
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <RTClib.h>

// ==============================================================================
// 1. HARDWARE PIN DEFINITIONS & INSTANCES
// ==============================================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 Pin: NodeMCU D5 corresponds to GPIO14
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// DS1307 RTC (I2C: SDA -> D2/GPIO4, SCL -> D1/GPIO5)
RTC_DS1307 rtc;

// Web Server listening on HTTP port 80
ESP8266WebServer server(80);

// Wi-Fi SoftAP Credentials
const char* ssid = "ESP-Clock";
const char* password = ""; // Open AP (enter password if security desired)

// ==============================================================================
// 2. GLOBAL STATE VARIABLES
// ==============================================================================
float temperature = 0.0;
float humidity = 0.0;
bool oledOK = false;
bool rtcOK = false;

// Non-blocking timing tracking (replaces delay(2000) for instant web responsiveness)
unsigned long lastSensorReadTime = 0;
const unsigned long SENSOR_INTERVAL = 2000; // Read sensors every 2000 ms

// Forward declarations
void handleRoot();
void handleSetTime();
void handleSetDate();
void updateSensorsAndDisplay();

// ==============================================================================
// 3. SETUP INITIALIZATION
// ==============================================================================
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(F("\n==========================================="));
  Serial.println(F("   ESP8266 SMART CLOCK & WEATHER MONITOR   "));
  Serial.println(F("==========================================="));

  // Initialize I2C Bus explicitly (NodeMCU: SDA=D2/GPIO4, SCL=D1/GPIO5)
  Wire.begin(D2, D1);

  // Initialize 0.96" OLED Display
  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    oledOK = true;
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(10, 20);
    display.println(F("ESP8266 IoT Clock"));
    display.setCursor(10, 35);
    display.println(F("System Initializing..."));
    display.display();
    Serial.println(F("[+] OLED Display initialized (0x3C)."));
  } else {
    Serial.println(F("[-] Warning: OLED Display not detected at 0x3C!"));
  }

  // Initialize DHT11 Sensor
  dht.begin();
  Serial.println(F("[+] DHT11 sensor initialized on pin D5."));

  // Initialize DS1307 Real-Time Clock
  if (rtc.begin()) {
    rtcOK = rtc.isrunning();
    if (!rtcOK) {
      Serial.println(F("[-] RTC initialized but oscillator is stopped! Adjusting to compile time..."));
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      rtcOK = true;
    } else {
      Serial.println(F("[+] DS1307 RTC running smoothly."));
    }
  } else {
    Serial.println(F("[-] Warning: DS1307 RTC module not found on I2C bus!"));
  }

  // Start Wi-Fi in SoftAP Mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print(F("[+] SoftAP active! SSID: "));
  Serial.println(ssid);
  Serial.print(F("[+] Dashboard URL: http://"));
  Serial.println(WiFi.softAPIP());

  // Register Web Routes
  server.on("/", handleRoot);
  server.on("/setTime", handleSetTime);
  server.on("/setDate", handleSetDate);
  server.begin();
  Serial.println(F("[+] HTTP Web Server running on port 80."));

  // Initial read and display update
  updateSensorsAndDisplay();
}

// ==============================================================================
// 4. WEB SERVER HANDLERS
// ==============================================================================
void handleRoot() {
  DateTime now = rtcOK ? rtc.now() : DateTime(2026, 1, 1, 0, 0, 0);

  String html = F("<!DOCTYPE html><html><head><title>ESP8266 Smart Dashboard</title>"
                  "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                  "<meta http-equiv='refresh' content='10'>"
                  "<style>"
                  "body{font-family:'Segoe UI',Arial,sans-serif;background:#0d1117;color:#c9d1d9;text-align:center;margin:0;padding:20px;}"
                  ".container{max-width:420px;margin:0 auto;}"
                  ".card{background:#161b22;border:1px solid #30363d;border-radius:12px;padding:15px;margin-bottom:15px;box-shadow:0 4px 12px rgba(0,0,0,0.3);}"
                  "h2{color:#58a6ff;margin-bottom:20px;}"
                  ".val{font-size:24px;font-weight:bold;color:#3fb950;margin:6px 0;}"
                  "input[type='time'],input[type='date']{background:#21262d;border:1px solid #30363d;color:#c9d1d9;padding:8px 12px;border-radius:6px;font-size:16px;}"
                  "input[type='submit']{background:#238636;color:#fff;border:none;padding:10px 18px;border-radius:6px;font-size:15px;font-weight:bold;cursor:pointer;margin-top:10px;}"
                  "input[type='submit']:hover{background:#2ea043;}"
                  ".footer{font-size:12px;color:#8b949e;margin-top:20px;}"
                  "</style></head><body><div class='container'>");

  html += F("<h2>ESP8266 Clock & Climate</h2>");

  // Environmental telemetry card
  html += F("<div class='card'><h3>🌡️ Temperature</h3><div class='val'>");
  html += (!isnan(temperature)) ? String(temperature, 1) + " &deg;C" : "Sensor Error";
  html += F("</div></div>");

  html += F("<div class='card'><h3>💧 Humidity</h3><div class='val'>");
  html += (!isnan(humidity)) ? String(humidity, 1) + " %" : "Sensor Error";
  html += F("</div></div>");

  // RTC time and date display
  if (rtcOK) {
    char timeBuf[12];
    sprintf(timeBuf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    char dateBuf[16];
    sprintf(dateBuf, "%02d/%02d/%04d", now.day(), now.month(), now.year());

    html += F("<div class='card'><h3>⏰ Current Time</h3><div class='val'>");
    html += String(timeBuf);
    html += F("</div><h3>📅 Current Date</h3><div class='val'>");
    html += String(dateBuf);
    html += F("</div></div>");
  } else {
    html += F("<div class='card'><p style='color:#f85149;'>⚠️ DS1307 RTC module offline</p></div>");
  }

  // Time calibration form
  html += F("<div class='card'><h3>Set New Time</h3>"
            "<form action='/setTime' method='get'>"
            "<input type='time' name='time' required><br/>"
            "<input type='submit' value='Update Time'></form></div>");

  // Date calibration form
  html += F("<div class='card'><h3>Set New Date</h3>"
            "<form action='/setDate' method='get'>"
            "<input type='date' name='date' required><br/>"
            "<input type='submit' value='Update Date'></form></div>");

  html += F("<div class='footer'>ESP8266 SoftAP Dashboard &bull; Live Telemetry</div>"
            "</div></body></html>");

  server.send(200, "text/html", html);
}

void handleSetTime() {
  if (server.hasArg("time") && rtcOK) {
    String timeStr = server.arg("time");
    int hour = timeStr.substring(0, 2).toInt();
    int minute = timeStr.substring(3, 5).toInt();

    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, 0));
    server.send(200, "text/html", F("<html><body style='font-family:sans-serif;text-align:center;padding:40px;background:#111;color:#fff;'><h3>Time Synchronized Successfully!</h3><p><a href='/' style='color:#58a6ff;'>&larr; Return to Dashboard</a></p></body></html>"));
  } else {
    server.send(400, "text/html", F("<html><body style='font-family:sans-serif;text-align:center;padding:40px;'><h3>Error: Invalid Input or RTC Offline!</h3><p><a href='/'>&larr; Back</a></p></body></html>"));
  }
}

void handleSetDate() {
  if (server.hasArg("date") && rtcOK) {
    String dateStr = server.arg("date");
    int year = dateStr.substring(0, 4).toInt();
    int month = dateStr.substring(5, 7).toInt();
    int day = dateStr.substring(8, 10).toInt();

    DateTime now = rtc.now();
    rtc.adjust(DateTime(year, month, day, now.hour(), now.minute(), now.second()));
    server.send(200, "text/html", F("<html><body style='font-family:sans-serif;text-align:center;padding:40px;background:#111;color:#fff;'><h3>Date Synchronized Successfully!</h3><p><a href='/' style='color:#58a6ff;'>&larr; Return to Dashboard</a></p></body></html>"));
  } else {
    server.send(400, "text/html", F("<html><body style='font-family:sans-serif;text-align:center;padding:40px;'><h3>Error: Invalid Input or RTC Offline!</h3><p><a href='/'>&larr; Back</a></p></body></html>"));
  }
}

// ==============================================================================
// 5. SENSOR SAMPLING & OLED RENDERING
// ==============================================================================
void updateSensorsAndDisplay() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (oledOK) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // Header banner
    display.setTextSize(1);
    display.setCursor(14, 0);
    display.print(F("IoT CLOCK & WEATHER"));
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    // Temperature & Humidity metrics
    display.setCursor(0, 16);
    if (!isnan(temperature) && !isnan(humidity)) {
      display.print(F("Temp: "));
      display.print(temperature, 1);
      display.println(F(" C"));

      display.setCursor(0, 28);
      display.print(F("Humidity: "));
      display.print(humidity, 1);
      display.println(F(" %"));
    } else {
      display.println(F("DHT11: Sensor Error"));
    }

    // Divider line
    display.drawLine(0, 39, 128, 39, SSD1306_WHITE);

    // RTC Time and Date
    if (rtcOK) {
      DateTime now = rtc.now();

      char timeBuf[10];
      sprintf(timeBuf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
      display.setCursor(0, 44);
      display.print(F("Time: "));
      display.println(timeBuf);

      char dateBuf[12];
      sprintf(dateBuf, "%02d/%02d/%04d", now.day(), now.month(), now.year());
      display.setCursor(0, 55);
      display.print(F("Date: "));
      display.println(dateBuf);
    } else {
      display.setCursor(0, 48);
      display.println(F("RTC Module Offline"));
    }

    display.display();
  }
}

// ==============================================================================
// 6. MAIN LOOP (NON-BLOCKING)
// ==============================================================================
void loop() {
  // Handle incoming HTTP client requests continuously without lag
  server.handleClient();

  // Periodically refresh sensor sampling & display using non-blocking millis()
  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorReadTime >= SENSOR_INTERVAL) {
    lastSensorReadTime = currentMillis;
    updateSensorsAndDisplay();
  }
}
