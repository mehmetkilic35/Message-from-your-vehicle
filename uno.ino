#include <Wire.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
// SoftwareSerial nesnesi tanımlama

SoftwareSerial mySerial(10, 11);

const int disableButtonPin = 2;
const int button1Pin = 3;
const int button2Pin = 4;
const int ledPin = 5;

const int gasSensorPin = A0;
const int airQualityThreshold = 30; //Hava Kalitesi kritik değeri 
//(0-50 iyi, 51-150 hassas, 151-200 sağlıksız, 201-300 çok sağlıksız, 301-500 tehlikeli)

const int dhtPin = 6;
const int criticalHumidity = 60;   // Nem kritik değeri (30-60 arası kabul edilebilir seviyeler)
const int criticalTemperature = 35;   //Yüksek sıcaklık kritik değeri
const int lowTemperature = 4;   // Düşük sıcaklık kritik değeri

const int fireSensorPin = 7;
const int fireThreshold = 300; // Ateş Algılayıcı Kritik değeri

const int vibrationSensorPin = A1;
const int vibrationThreshold = 500; // Titreşim Kritik Değeri

bool disableButtonPressed = false;

DHT dht(dhtPin, DHT22);

void setup() {
  pinMode(disableButtonPin, INPUT);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);
  Serial.begin(9600);
  
  dht.begin();
  
}

void loop() {
  disableButtonPressed = (digitalRead(disableButtonPin) == LOW);
  
  int disableButtonState = digitalRead(disableButtonPin);
  int button1State = digitalRead(button1Pin);
  int button2State = digitalRead(button2Pin);

  if (disableButtonPressed) { // Disable butonuna basılıysa
    digitalWrite(ledPin, LOW); // LED'i kapat
  } else { // Disable butonuna basılı değilse
    float humidity = dht.readHumidity(); // Nem değerini oku
    float temperature = dht.readTemperature(); // Sıcaklık değerini oku (Celsius)
        
    if (button1State == LOW || button2State == LOW) { // Button1 veya Button2 basılıysa
      digitalWrite(ledPin, HIGH); // LED'i aç
                
      
      int gasSensorValue = analogRead(gasSensorPin); // Gaz sensörü değerini oku
      Serial.print("Gaz sensor degeri: ");
      Serial.println(gasSensorValue);

      int vibrationSensorValue = analogRead(vibrationSensorPin); // Titreşim sensörü değerini oku
      Serial.print("Titresim sensor degeri: ");
      Serial.println(vibrationSensorValue);

      bool sendAlert = false;

      if (vibrationSensorValue > vibrationThreshold) {
        Serial.println("Titreşim algılandı!");
        sendCriticalCondition("Titreşim", vibrationSensorValue);
      }
      
      if (gasSensorValue >= airQualityThreshold) {
        Serial.println("Hava kalitesi kritik seviyede");
        sendCriticalCondition("Hava kalitesi", gasSensorValue);
      }

      
      Serial.print("Nem: ");
      Serial.print(humidity);
      Serial.print(" %\t");

      
      Serial.print("Sicaklik: ");
      Serial.print(temperature);
      Serial.println(" *C");

      if (humidity > criticalHumidity) {
        Serial.println("Nem değeri kritik seviyede");
        sendCriticalCondition("Nem", humidity);
      }

      if (temperature > criticalTemperature) {
        Serial.println("Çok sıcak");
        sendCriticalCondition("Sıcaklık", temperature);
      } else if (temperature < lowTemperature) {
        Serial.println("Çok soğuk");
        sendCriticalCondition("Sıcaklık", temperature);
      
      }
      int fireSensorValue = analogRead(fireSensorPin); // Ateş sensörü değerini oku
      Serial.print("Ates sensor degeri: ");
      Serial.println(fireSensorValue);

      if (fireSensorValue >= fireThreshold) {
        Serial.println("Ateş algılandı!");
        sendCriticalCondition("Ateş", fireSensorValue);
      }
      
     
      delay(1000);
    } else {
      digitalWrite(ledPin, LOW); // LED'i kapat
    }
  }
}


void sendCriticalCondition(String condition, float value) {
  StaticJsonDocument<200> doc;
  doc["condition"] = condition;
  doc["value"] = value;

 // Serialize and print the JSON data to the Serial monitor
  serializeJson(doc, Serial);
  Serial.println();

  // Send the JSON data to the ESP32
  serializeJson(doc, mySerial);
  mySerial.println();
}

