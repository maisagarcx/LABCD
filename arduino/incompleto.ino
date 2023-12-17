#include "DHT.h" // Use version 1.4.2 of DHT Sensor Library
#include "RTClib.h" // Use version 1.2.0 RTClib
// Also, I installed version 1.0.2 of Adafruit Unified Sensor
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define DHTPIN 6
#define DHTTYPE DHT11
#define SERVOPIN 10
#define DELAYSERVO 20
#define LAMPPIN 13

RTC_DS3231 rtc;
Servo myServo;
DHT dht(DHTPIN, DHTTYPE);
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
float position = 0;

byte waterDrop[] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110
};

byte thermometer[] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte clock[] = {
  B00000,
  B00000,
  B01110,
  B10011,
  B10101,
  B10001,
  B01110,
  B00000
};

byte lampOff[] = {
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B01110,
  B01110,
  B00100
};

byte lampOn[] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B01010,
  B01110,
  B00100
};

void setup(){

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.createChar(0, thermometer);
  lcd.createChar(1, waterDrop);
  lcd.createChar(3, clock);
  lcd.createChar(4, lampOff);
  lcd.createChar(5, lampOn);
  dht.begin();
  myServo.attach(SERVOPIN);
  myServo.write(position);

  if (!rtc.begin()){
    Serial.println("Couldn't find RTC");
    return;
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop(){

  delay(3000);
  DateTime now = rtc.now();
  int lamp;
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(false); // Set true for Fahrenheit

  if (isnan(hum) || isnan(temp)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  if (temp <= 37.8){
    digitalWrite(LAMPPIN, HIGH);
    lamp = 1;
  }
  else{
    digitalWrite(LAMPPIN, LOW);
    lamp = 0;
  }

  showSerialMonitor(temp, hum, now, lamp);
  showLDC(temp, hum, now, lamp);
  moveServoUp();
  moveServoDown();

}

void showSerialMonitor(float T, float H, DateTime now, int L){

  Serial.print("Lamp: ");
  Serial.println(L);
  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(H);
  Serial.println("%");
  
  int hour = now.hour();
  int minute = now.minute();

  Serial.print("Hour: ");

  if(hour < 10)
    Serial.print("0");

  Serial.print(hour, DEC);
  Serial.print(":");

  if(minute < 10)
    Serial.print("0");

  Serial.println(minute, DEC);
  Serial.println();

}

void showLDC(float T, int H, DateTime now, int L){
  
  lcd.setCursor(1, 0);
  lcd.write(byte(0));
  lcd.print(T);
  lcd.write(byte(223));
  lcd.print("C");

  lcd.setCursor(11, 0);
  lcd.write(byte(1));
  lcd.print(H);
  lcd.print("%");

  lcd.setCursor(1, 1);
  lcd.write(byte(3));

  int hour = now.hour();
  int minute = now.minute();

  if(hour < 10)
    lcd.print("0");

  lcd.print(hour, DEC);
  lcd.print(":");

  if(minute < 10)
    lcd.print("0");

  lcd.print(minute, DEC);

  lcd.setCursor(11, 1);
  if(!L){
    lcd.write(byte(4));
    lcd.print("OFF");
  }
  else{
    lcd.write(byte(5));
    lcd.print("ON ");
  }

}

void moveServoUp(){

  for (position = 0; position <= 180; position += 1){ 
    myServo.write(position);
    delay(DELAYSERVO);
  }

}

void moveServoDown(){

  for (position = 180; position >= 0; position -= 1){
    myServo.write(position);      
    delay(DELAYSERVO);
  }
  
}
