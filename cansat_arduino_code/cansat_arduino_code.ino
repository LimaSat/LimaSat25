#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#include <math.h>

#define SD_CS 10

#define SEALEVELPRESSURE_HPA (1018.7)

Adafruit_BME280 bme;
Adafruit_BMP280 bmp;


File file;

//sets the delay on loop
unsigned long delayTime = 700;

// buzzer
const int buzzer = 2;
bool buzzer_timing = true;


// Change this to true if you want to save the data to an SD Card 
bool sdCard = true;

// Both should start as false, later in the code only one of them will be changed to know wich sensor is being used
bool isBME = false;
bool isBMP = false;

// This is the name of the file that will be created if sdCard = True
String fileName = "sensores.csv";

// Creates a custom struct to save the bme280 or bmp280 data
struct BMEPData {
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

  // pin buzzer
  pinMode(buzzer, OUTPUT);

  if (sdCard){
    // Initializes the sd card module
    pinMode(SD_CS, OUTPUT);
    bool sd_status = SD.begin(SD_CS);
    // Checks if the SD card was initialized and stops the program if not
    if (!sd_status) {
      Serial.println("SD Card initialization failed!");
      sdCard = false;
    }
    else {
      // creates/opens a file in the sd card
      file = SD.open(fileName, FILE_WRITE);

      // Checks if the file was created
      if (file) {
        file.println("Temperature, Pressure, Altitude, UV1, UV2, UV3, UV4, MaxUV, Time, Sum, Checksum"); // Header for the csv file
      }
      else {
          Serial.println("no file");
      }

      // Closes the file
      file.close();
    }
  }

  // Initializes the BME280 or the BMP280 sensor
  isBME = bme.begin();
  isBMP = bmp.begin();
}


void loop() {

  buzzer_sound();
  
  // Saves the BME sensor data
  BMEPData bmepData = bmepValues();

  float temperature = bmepData.temperature;
  float pressure = bmepData.pressure;



  float altitude = bmepData.altitude;


  // Saves the UV sensor data to an array 
  float sensorUVdata[4];

  sensorUVdata[0] = uvValues(A0);
  sensorUVdata[1] = uvValues(A1);
  sensorUVdata[2] = uvValues(A2);
  sensorUVdata[3] = uvValues(A3);


  // Chooses the highest value from across the 4 UV sensors and saves it
  float sumUV = 0;
  for(int i = 0; i < 4; i++){ 
    sumUV += sensorUVdata[i];
  }

  // Time in milliseconds
  unsigned long time_ms = millis();


  // CheckSum

  float data[] = {temperature, pressure, altitude,  sensorUVdata[0], sensorUVdata[1], sensorUVdata[2], sensorUVdata[3], sumUV, time_ms};
  int size = sizeof(data) / sizeof(data[0]);  // Calculate number of elements in the array

  float sum = 0;  // Variable to hold the total sum

  for (int i = 0; i < size; i++) {
    sum += data[i]; // Multiply each value by 100 to preserve 2 decimal places, then convert to integer
  }
  sum *=100;
  unsigned long sum2 = round(sum);
  byte checksum = sum2 % 256;  // Compute 1-byte checksum (modulo 256)



  String data_values = String(temperature) + "," +
  String(pressure) + "," +
  String(altitude) + "," +
  String(sensorUVdata[0]) + "," +
  String(sensorUVdata[1]) + "," +
  String(sensorUVdata[2]) + "," +
  String(sensorUVdata[3]) + "," +
  String(sumUV) + "," +
  String(time_ms) + "," +
  String(sum2) + ","+
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


// Returns the values received from the bme280 or the bmp280
BMEPData bmepValues() {
  BMEPData data;
  if (isBME) {
    data.temperature = bme.readTemperature();
    data.pressure = (bme.readPressure() / 100.0F) * 100; // pascais!!!
    data.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  }
  else if (isBMP) {
    data.temperature = bmp.readTemperature();
    data.pressure = (bmp.readPressure() / 100.0F) * 100;
    data.altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  }
  else {
    data.temperature = NAN;
    data.pressure = NAN;
    data.altitude = NAN;
  }
  

  return data;
}


// Returns the values received from a uv sensor
float uvValues(int pin) {
  float sensorUV = (float)analogRead(pin) * 5000 / 1023.0;

  return sensorUV;
}

void buzzer_sound() {
  if (buzzer_timing) {
    buzzer_timing = false;
    tone(buzzer, 1000);
  }
  else {
    buzzer_timing = true;
    noTone(buzzer); 
  }
}



