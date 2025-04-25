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
unsigned long delayTime = 700;


// Change this to true if you want to save the data to an SD Card 
bool sdCard = false;

// This is the name of the file that will be created if sdCard = True
String fileName = "sensores.csv";


struct BMEData {
  float temperature;
  float pressure;
  float altitude;
};


void setup() {
  Serial.begin(9600);
  delay(2000);

  // Prepare to send data
  Wire.begin();

  // configure the analog pins for the uv sensors
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

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
      file.println("Temperature, Pressure, Altitude, UV1, UV2, UV3, UV4, MaxUV, Time"); // Header for the csv file
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

  if (!status) {
    Serial.println("BME initialization failed, check wiring!");
    while(1);
  }

}


void loop() {
  
  // Saves the BME sensor data
  BMEData bmeData = bmeValues();

  float temperature = bmeData.temperature;
  float pressure = bmeData.pressure;
  float altitude = bmeData.altitude;


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

  // Time in milliseconds
  unsigned long time_ms = millis();


  // CheckSum

  float data[] = {temperature, pressure, altitude,  sensorUVdata[0], sensorUVdata[1], sensorUVdata[2], sensorUVdata[3], maxUV, time_ms};
  int size = sizeof(data) / sizeof(data[0]);  // Calculate number of elements in the array

  unsigned long sum = 0;  // Variable to hold the total sum

  for (int i = 0; i < size; i++) {
    sum += (unsigned long)(data[i] * 100); // Multiply each value by 100 to preserve 2 decimal places, then convert to integer
  }

  byte checksum = sum % 256;  // Compute 1-byte checksum (modulo 256)



  String data_values = String(temperature) + "," +
  String(pressure) + "," +
  String(altitude) + "," +
  String(sensorUVdata[0]) + "," +
  String(sensorUVdata[1]) + "," +
  String(sensorUVdata[2]) + "," +
  String(sensorUVdata[3]) + "," +
  String(maxUV) + "," +
  String(time_ms) + "," +
  String(checksum);

  // Sending data to APC220
  Serial.println(data_values);
  
  
  // Write to SD card
  if (sdCard) {
    file = SD.open(fileName, FILE_WRITE);

    file.println(data_values);

    file.close();
  }

  delay(delayTime); 
}
// End of loop


// Returns the values received from the bme280
BMEData bmeValues() {
  BMEData data;
  data.temperature = bme.readTemperature();
  data.pressure = bme.readPressure() / 100.0F;
  data.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  return data;
}


// Returns the values received from a uv sensor
float uvValues(int pin) {
  float sensorUV = (float)analogRead(pin) * 5000 / 1023.0;

  return sensorUV;
}



