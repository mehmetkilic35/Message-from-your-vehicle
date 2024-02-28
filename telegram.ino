/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-esp32-motion-detection-arduino/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Arac";
const char* password = "mesajvar";

// Initialize Telegram BOT
#define BOTtoken "5272818533:AAFCkH0ydiA03ij7ylK95jSy0lHgV_EUYNw"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "5236074344"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOTtoken, secured_client);

const int buton = 27; // PIR Motion Sensor

/*
// Indicates when motion is detected
void IRAM_ATTR detectsMovement() {
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}
*/
void setup() {
  
  Serial.begin(115200);

  // PIR Motion Sensor mode INPUT_PULLUP
  //pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  //attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
  pinMode(buton, INPUT);
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  //secured_client.setInsecure();
  
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  //Serial.print("Okunan deger ");
  //Serial.println(digitalRead(buton));

  if( digitalRead(buton) == 1 ){
    if(bot.sendMessage(CHAT_ID, "Lutfen **35 AA 111** plakalı aracinizin basina gidiniz.", ""))
    {
      Serial.println("telegrama mesaj gonderildi.");
    }
    else
    {
      Serial.println("telegrama mesaj GONDERILEMEDI.");
    }
    Serial.println("Lutfen **35 AA 111** plakalı aracinizin basina gidiniz.");
    //motionDetected = false;
    
  
    delay(1000);
  }
  }
