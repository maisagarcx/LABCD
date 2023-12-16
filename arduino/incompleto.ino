#include "DHT.h" // Use version 1.4.2 of DHT Sensor Library
#include "RTClib.h" // Use version 1.2.0 RTClib
// Also, I installed version 1.0.2 of Adafruit Unified Sensor
#include <Wire.h>
#include <LiquidCrystal.h>
// #include <Servo.h>

#define DHTPIN 3
#define DHTTYPE DHT11

RTC_DS3231 rtc;
// Servo myServo;
DHT dht(DHTPIN, DHTTYPE);
const int RS = 12, EN = 11, D4 = 10, D5 = 9, D6 = 8, D7 = 7;
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup(){

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  dht.begin();
  // myServo.attach(5);
  if (!rtc.begin()){
    Serial.println("Couldn't find RTC");
    return;
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop(){

  delay(5000);
    
  DateTime now = rtc.now();
  float position = 0;
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(false); // Set true for Fahrenheit

  if (isnan(hum) || isnan(temp)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  showSerialMonitor(temp, hum, now);
  showLDC(temp, hum, now);
  // moveServoUp(position);
  // moveServoDown(position);

}

void showSerialMonitor(float T, float H, DateTime now){

  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(H);
  Serial.println("%");
  Serial.print("Date: ");
  Serial.print(now.day(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.println(now.year(), DEC);
  Serial.print("Hour: ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.println(now.minute(), DEC);

}

void showLDC(float T, float H, DateTime now){
  
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(T);
  lcd.print(" H:");
  lcd.print(H);

  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);

}

void moveServoUp(float *position){

  Serial.println(*position);
  for (position; position <= 180; position += 1){ 
    // myServo.write(position);             
    delay(15);                     
  }

  Serial.println(*position);
  lcd.setCursor(5,1);
  lcd.print(*position);
  lcd.write(byte(223));

}

void moveServoDown(float *position){

  Serial.println(*position);
  for (position; position >= 0; position -= 1){
    // myServo.write(position);           
    delay(15);  
  }

  Serial.println(*position);
  lcd.setCursor(5,1);
  lcd.print(*position);
  lcd.write(byte(223));
                  
}
