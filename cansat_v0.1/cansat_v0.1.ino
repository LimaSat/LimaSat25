#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;


//sets the delay on loop
unsigned long delayTime = 1000;


// Change this to true for CSV format or false for readable output
bool csvMode = true;


void setup() {
  Serial.begin(9600);

  bool status;
  status = bme.begin();  //starts the bme280 sensor I think

  // DO NOT FORGET THAT IF CSV MODE IS ENABLED IT DOESNT CHECK IF BME280 IS CONNECTED
  if (!csvMode) {
    Serial.println(F("BME280 test"));
  
    //checks if bme280 sensor is well connected
    if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }
  
    Serial.println("-- Default Test --");
  }
}


//serial prints the values received from bme280
void bmeValues() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.readHumidity();

  if (csvMode) {
    Serial.print(temperature); Serial.print(",");
    Serial.print(pressure); Serial.print(",");
    Serial.print(altitude); Serial.print(",");
    Serial.print(humidity); Serial.print(",");
  } else {
    Serial.print("Temperature = "); Serial.print(temperature); Serial.println(" *C");
    Serial.print("Pressure = "); Serial.print(pressure); Serial.println(" hPa");
    Serial.print("Approx. Altitude = "); Serial.print(altitude); Serial.println(" m");
    Serial.print("Humidity = "); Serial.print(humidity); Serial.println(" %");
  }
}


//serial prints the values received from uv sensor
void uvValues() {
  float sensorUV = analogRead(A1) * 5000 / 1023.0;

  if (csvMode) {
    Serial.print(sensorUV); Serial.print(",");
  } else {
    Serial.print("UV Sensor = "); Serial.print(sensorUV); Serial.println(" mV");
    Serial.println();
  }
}


void loop() {
  
  bmeValues();

  uvValues();
  
  unsigned long time_ms = millis();
  
  if (csvMode) {
    Serial.println(time_ms); // Last value with newline in csv file
  } 
  else {
    Serial.print("Time = "); Serial.print(time_ms); Serial.println(" ms");
    Serial.println();
  }
  
  delay(delayTime); 
}
