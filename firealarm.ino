#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "MuSTaQiM";          // Replace with your WiFi SSID
const char* password = "mustaqim0919";  // Replace with your WiFi password

// Telegram Bot credentials
String botToken = "7994581873";
String chatID = "-1002401346969";  // Replace with your group chat ID

// WiFiClientSecure client;
// UniversalTelegramBot bot(BOTtoken, client);

// Sensor pins
#define FLAME_SENSOR_PIN 34   // Analog pin for flame sensor
#define FLAME_DIGITAL_PIN 32  // Digital pin for flame sensor
#define SMOKE_SENSOR_PIN 35   // Analog pin for smoke sensor
#define SMOKE_DIGITAL_PIN 33  // Digital pin for smoke sensor

// Warning light relay pin
#define WARNING_LIGHT_RELAY_PIN 25  // GPIO pin for relay controlling the 12V LED

// Variables to track alert state
bool flameDetected = false;
bool smokeDetected = false;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Initialize sensor pins
  pinMode(FLAME_DIGITAL_PIN, INPUT);
  pinMode(SMOKE_DIGITAL_PIN, INPUT);
  pinMode(WARNING_LIGHT_RELAY_PIN, OUTPUT);  // Initialize relay control pin
}

void loop() {
  // Read flame sensor values
  int flameAnalogValue = analogRead(FLAME_SENSOR_PIN);
  int flameDigitalValue = digitalRead(FLAME_DIGITAL_PIN);

  // Read smoke sensor values
  int smokeAnalogValue = analogRead(SMOKE_SENSOR_PIN);
  int smokeDigitalValue = digitalRead(SMOKE_DIGITAL_PIN);

  // Warning light logic
  if (flameDigitalValue == LOW || smokeDigitalValue == LOW) {  // Flame or smoke detected
    if (!flameDetected && flameDigitalValue == LOW) {
      Serial.println("🔥 Alert! Flame detected!");
      // bot.sendMessage(CHAT_ID, "🔥 Alert! Flame detected!", "");
      sendMessageToTelegram("🔥 Alert! Flame detected!");

      flameDetected = true;
    }
    if (!smokeDetected && smokeDigitalValue == LOW) {
      Serial.println("💨 Alert! Smoke detected!");
      // bot.sendMessage(CHAT_ID, "💨 Alert! Smoke detected!", "");
      sendMessageToTelegram("💨 Alert! Smoke detected!");

      smokeDetected = true;
    }
    Serial.println("🚨 Alert! Warning light ON!");

    digitalWrite(WARNING_LIGHT_RELAY_PIN, LOW);  // Turn on the relay to activate the 12V warning light
    delay(1000);                                 // Optional: Add delay to control light behavior
  } else {
    if (flameDetected || smokeDetected) {
      Serial.println("All clear. No flame or smoke detected. Warning light OFF.");
      // bot.sendMessage(CHAT_ID, "✅ All clear. No flame or smoke detected.", "");
      sendMessageToTelegram("✅ All clear. No flame or smoke detected.");

      flameDetected = false;
      smokeDetected = false;
    }
    digitalWrite(WARNING_LIGHT_RELAY_PIN, HIGH);  // Turn off the relay to deactivate the 12V warning light
    delay(1000);                                  // Optional: Add delay for turning light off
  }

  // Log sensor data to Serial Monitor
  Serial.print("Flame (Analog): ");
  Serial.println(flameAnalogValue);
  Serial.print("Flame (Digital): ");
  Serial.println(flameDigitalValue);
  Serial.print("Smoke (Analog): ");
  Serial.println(smokeAnalogValue);
  Serial.print("Smoke (Digital): ");
  Serial.println(smokeDigitalValue);
  Serial.println("-------------------------------");

  delay(500);  // Main loop delay to prevent spamming
}


void sendMessageToTelegram(String message) {
  String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + message;

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();  // Send GET request

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Message sent successfully!");
    Serial.println(payload);  // Print response for debugging
  } else {
    Serial.println("Error sending message");
  }

  http.end();  // Close connection
}