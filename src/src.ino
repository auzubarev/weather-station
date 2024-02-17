#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>


OneWire oneWire(D9);
DallasTemperature sensors(&oneWire);
Adafruit_BMP085 bmp;
Adafruit_AHTX0 aht;


void setup(void) {
  Serial.begin(115200);
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

  delay(5000);

}