#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Telegram Bot Token and Group Chat ID
String botToken = "7994581873:AAF3r1Z4DRVhToLokWzlqZVXUcdM4_1bG3A";
String chatID = "-1002401346969";  // Replace with your group chat ID

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Send the message to the Telegram group
  sendMessageToTelegram(message);
}

void loop() {
  // Nothing in the loop for now
}

void sendMessageToTelegram(String message) {
  String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + "";

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
