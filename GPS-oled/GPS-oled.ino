/*****************************************
* ESP32 GPS VKEL 9600 Bds
******************************************/

#include <TinyGPS++.h>                       
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h

TinyGPSPlus gps;                            
//HardwareSerial Serial1(1);                 

void setup()
{
  Serial.begin(115200);
//  Serial1.begin(9600, SERIAL_8N1, 17, 18);   //17-TX 18-RX
  Serial1.begin(9600, SERIAL_8N1, 34, 12);   //17-TX 18-RX
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

void loop()
{
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  Serial.print("Latitude  : ");
  Serial.println(gps.location.lat(), 5);
//  display.drawString(0, 10, "Lat: " + String(gps.location.lat()));
  Serial.print("Longitude : ");
  Serial.println(gps.location.lng(), 4);
//  display.drawString(0, 20, "Long: " + String(gps.location.lng()));

  Serial.print("Satellites: ");
  Serial.println(gps.satellites.value());
  display.drawString(0, 30, "Sats: " + String(gps.satellites.value()));

  Serial.print("Altitude  : ");
  Serial.print(gps.altitude.feet() / 3.2808);
  Serial.println("M");
  Serial.print("Time      : ");
  Serial.print(gps.time.hour());
  Serial.print(":");
  Serial.print(gps.time.minute());
  display.drawString(0, 40, "Minute: " + String(gps.time.minute()));
  Serial.print(":");
  Serial.println(gps.time.second());
  Serial.println("**********************");
  display.display();

  smartDelay(1000);                                      

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

static void smartDelay(unsigned long ms)                
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}
