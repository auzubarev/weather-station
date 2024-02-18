#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "conf.h"


OneWire oneWire(D9);
DallasTemperature sensors(&oneWire);
Adafruit_BMP085 bmp;
Adafruit_AHTX0 aht;
HTTPClient http;
WiFiClient wifiClient;


void setup(void) {


  Serial.begin(115200);

  Serial.println("Connecting to WiFi");

  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("...Connecting to WiFi");
    delay(1000);
  }
  Serial.println("Connected");

  sensors.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) delay(10);
  }
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
}

void loop(void) {
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  Serial.println(tempC);

  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
    
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
//
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

  http.begin(wifiClient, API_URL);
  http.addHeader("Content-Type", "application/json");
  Serial.print("Server response: ");

  String json = String("{");
  json.concat("\"temperature\" : ");
  json.concat(sensors.getTempCByIndex(0));
  json.concat(", ");

  json.concat("\"humidity\" : ");
  json.concat(humidity.relative_humidity);
  json.concat(", ");
  
  json.concat("\"pressure\" : ");
  json.concat(bmp.readPressure()*0.007501);

  json.concat("}");
  
  Serial.print(http.POST(json));
  delay(5000);

}