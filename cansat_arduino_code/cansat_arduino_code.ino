#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SD.h>


#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

int CS_PIN = 10;

File file;

//sets the delay on loop
unsigned long delayTime = 1000;


// Change this to true for CSV format or false for readable output
bool csvMode = true;

// Change this to true if you want to save the data to an SD Card 
bool sdCard = true;

// This is the name of the file that will be created if sdCard = True
String fileName = "sensores.csv";

void setup() {
  Serial.begin(9600);

  if (sdCard){
    // Initializes the sd card module
    pinMode(CS_PIN, OUTPUT);
    SD.begin(CS_PIN);
    // Checks if the SD card was initialized and stops the program if not
    if (!SD.begin(CS_PIN)) {
      Serial.println("SD Card initialization failed!");
      while(1);
    }
    
    // creates/opens a file in the sd card
    file = SD.open(fileName, FILE_WRITE);
    // Checks if the file was created and stops the program if not
    if (file) {
      file.println("Temperature, Pressure, Altitude, UV1, UV2, UV3, UV4, MaxUV, Time");
    }
    else {
        Serial.println("no file");
        while(1);
    }

    // Closes the file to prevent data corruption
    file.close();
  }

  // Initializes the BME280 sensor
  bool status;
  status = bme.begin();  

  // DO NOT FORGET THAT IF CSV MODE IS ENABLED IT DOESNT CHECK IF BME280 IS CONNECTED
  if (!csvMode) {
    Serial.println(F("BME280 test"));
  
    
    // Checks if bme280 sensor is well connected
    while(!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      status = bme.begin();
      delay(delayTime);
    }

  
    Serial.println("-- Default Test --");
  }
}


void loop() {
  
  bmeValues();

  // Saves the UV sensor data to an array 
  float sensorUVdata[4];

  sensorUVdata[0] = uvValues(A0);
  sensorUVdata[1] = uvValues(A1);
  sensorUVdata[2] = uvValues(A2);
  sensorUVdata[3] = uvValues(A3);


  // Chooses the highest value from across the 4 UV sensors and saves it
  float maxUV = -100;
  for(int i = 0; i < 4; i++){
    if (sensorUVdata[i] > maxUV){
      maxUV = sensorUVdata[i];
    }
  }
  


  // Writes the max value from the UV sensors if working with the sd card
  if (sdCard) {
    file = SD.open(fileName, FILE_WRITE);
    file.print(maxUV);file.print(",");
    file.close();
  }

  // Prints to serial monitor the max value from the UV sensors
  if (csvMode) {
    Serial.print(maxUV); Serial.print(",");
  } 
  else {
    Serial.print("Max UV Sensor = "); Serial.print(maxUV); Serial.println(" mV");
  }

  
  unsigned long time_ms = millis();
  
  // Writes the time to the sd card
  if (sdCard) {
    file = SD.open(fileName, FILE_WRITE);
    file.print(time_ms);file.println(",");
    file.close();
  }
  

  // Prints to serial monitor the time
  if (csvMode) {
    Serial.println(time_ms); // Last value with newline in csv file
  } 
  else {
    Serial.print("Time = "); Serial.print(time_ms); Serial.println(" ms");
    Serial.println();
  }

  
  delay(delayTime); 
}


// Writes to sd card and serial prints the values received from bme280
void bmeValues() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  if (sdCard){
    file = SD.open(fileName, FILE_WRITE);
    file.print(temperature); file.print(",");
    file.print(pressure); file.print(",");
    file.print(altitude); file.print(",");
    file.close();
  }

  if (csvMode) {
    Serial.print(temperature); Serial.print(",");
    Serial.print(pressure); Serial.print(",");
    Serial.print(altitude); Serial.print(",");
  } 
  else {
    Serial.print("Temperature = "); Serial.print(temperature); Serial.println(" *C");
    Serial.print("Pressure = "); Serial.print(pressure); Serial.println(" hPa");
    Serial.print("Approx. Altitude = "); Serial.print(altitude); Serial.println(" m");
  }
}


// Writes to sd card and serial prints the values received from uv sensor
float uvValues(int pin) {
  float sensorUV = (float)analogRead(pin) * 5000 / 1023.0;
  
  if (sdCard){
    file = SD.open(fileName, FILE_WRITE);
    file.print(sensorUV); file.print(",");
    file.close();
  }

  if (csvMode) {
    Serial.print(sensorUV); Serial.print(",");
  } 
  else {
    Serial.print("UV Sensor = "); Serial.print(sensorUV); Serial.println(" mV");
  }
  return sensorUV;
}



