#include "DHT.h" // Use version 1.4.2 of DHT Sensor Library
#include "RTClib.h" // Use version 1.2.0 RTClib
// Also, I installed version 1.0.2 of Adafruit Unified Sensor
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define DHTPIN 6
#define DHTTYPE DHT11
#define SERVOPIN 10
#define DELAYSERVO 20
#define LAMPPIN 13

void showSerialMonitor(float T, float H, DateTime now, int L);
void showLDC(float T, int H, DateTime now, int L);

LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS3231 rtc;
Servo myServo;
DHT dht(DHTPIN, DHTTYPE);
float position = 0;
int day_one;

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

byte clk[] = {
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
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.createChar(0, thermometer);
  lcd.createChar(1, waterDrop);
  lcd.createChar(3, clk);
  lcd.createChar(4, lampOff);
  lcd.createChar(5, lampOn);
  dht.begin();
  myServo.attach(SERVOPIN);
  myServo.write(position);

  if (!rtc.begin()){
    Serial.println("Couldn't find RTC");
    return;
  }

  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(2023, 12, 18, 23, 59, 45));
  DateTime now = rtc.now();
  day_one = now.day();
  
}

void loop(){

  delay(1000);
  DateTime now = rtc.now();
  
  int last = 21;
  int new_day = now.day();
  
  if(new_day != day_one){
    Serial.print("Last: ");
    Serial.println(last);
    Serial.print("Past day: ");
    Serial.println(day_one);
    last = last - 1;
    day_one = new_day;
    Serial.print("New day: ");
    Serial.println(day_one);
    Serial.print("Last: ");
    Serial.println(last);
    Serial.println();
  }
  
  int lamp = 0;
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(false); // Set true for Fahrenheit
  int currentHour = now.hour();
  int currentMinute = now.minute();
  int currentSecond = now.second();

  if (isnan(hum) || isnan(temp)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  if (last >= 3){ 
    if (temp <= 37.4){
      digitalWrite(LAMPPIN, HIGH);
      lamp = 1;
    }
    else if (temp >= 37.8){
      digitalWrite(LAMPPIN, LOW);
      lamp = 0;
    }
  }
  else{
    if (temp <= 36.4){
      digitalWrite(LAMPPIN, HIGH);
      lamp = 1;
    }
    else if (temp >= 36.8){
      digitalWrite(LAMPPIN, LOW);
      lamp = 0;
    }
  }

  int startAngles[] = {0, 45, 90, 135, 180, 135, 90, 45};
  int endAngles[] = {45, 90, 135, 180, 135, 90, 45, 0};

  if (last >= 3){
    for (int i = 0; i < 8; ++i){
      if (currentHour == i * 3 && currentMinute == 0 && currentSecond == 0){
        moveServo(&startAngles[i], &endAngles[i], 1);
        break;
      }
    }  
  }
  
  showSerialMonitor(temp, hum, now, lamp);
  showLDC(temp, hum, now, lamp);
  
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

void moveServo(int *start, int *end, int increment){
  
  if (*start < *end){
    for (int position = *start; position <= *end; position += increment){
      myServo.write(position);
      delay(DELAYSERVO);
    }
  } 
  else{
    for (int position = *start; position >= *end; position -= increment){
      myServo.write(position);
      delay(DELAYSERVO);
    }
  }
  
}
