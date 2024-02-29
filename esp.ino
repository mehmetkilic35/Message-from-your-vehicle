#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

String data = "";

const int airQualityThreshold = 30;
const int criticalHumidity = 10;
const int criticalTemperature = 35;
const int lowTemperature = 4;
const int fireThreshold = 300;
const int vibrationThreshold = 500;

// Replace with your network credentials
const char* ssid = "XeraP";
const char* password = "xera2006";

// Replace with your Bot token and Chat ID
const char* botToken = "6103262182:AAFmih16vFY8gGukQ4eJ5XrqPXt3MVcAsoI";
String chat_id = "1323943977";

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  Serial.println("Attempting to connect to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure time
  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", "UTC", 1);
  tzset();
  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX: 16, TX: 17
}

void loop() {
  static String data = "";
  while (Serial2.available()) {
    char incomingChar = (char)Serial2.read();
    if (incomingChar == '\n') {
      data.trim();
      Serial.println("Received data from Arduino UNO:");
      Serial.println(data);

      // Deserialize the received JSON data
      DynamicJsonDocument doc(200);
      DeserializationError error = deserializeJson(doc, data);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        data = "";
        return;
      }

      

      // Extract condition and value from the JSON data
      String condition = doc["condition"];
      float value = doc["value"];

      String message = "";

      if (condition == "Titreşim") {
        message += "Titreşim algılandı!\n";
      }
      if (condition == "Hava kalitesi") {
        message += "Hava kalitesi kritik seviyede\n";
      }
      if (condition == "Nem") {
        message += "Nem değeri kritik seviyede\n";
      }
      if (condition == "Sıcaklık") {
        if (value > criticalTemperature) {
          message += "Çok sıcak\n";
        } else if (value < lowTemperature) {
          message += "Çok soğuk\n";
        }
      }
      if (condition == "Ateş") {
        message += "Ateş algılandı!\n";
      }

      // If there is any critical message, send it via Telegram
      if (message.length() > 0) {
        Serial.println("Attempting to send Telegram message...");
        bool result = bot.sendMessage(chat_id, message);
        if (result) {
          Serial.println("Message sent successfully!");
        } else {
          Serial.println("Failed to send message!");
        }
      }

      data = ""; // Clear the data string
    } else {
      data += incomingChar;
    }
  }
}
