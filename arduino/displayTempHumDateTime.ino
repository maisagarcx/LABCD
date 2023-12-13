#include "DHT.h" // Use version 1.4.2 of DHT Sensor Library
#include "RTClib.h" // Use version 1.2.0 RTClib
// Also, I installed version 1.0.2 of Adafruit Unified Sensor
#include <Wire.h>

RTC_DS3231 rtc;
#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  rtc.begin();
  Serial.println("Setting the time...");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  delay(2000);
  DateTime now = rtc.now();
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(false);

  if (isnan(hum) || isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(hum);
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
