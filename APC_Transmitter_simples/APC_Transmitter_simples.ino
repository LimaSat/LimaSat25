#include <Wire.h>


void setup() {
    Serial.begin(9600);  // Serial communication with APC220
    delay(2000);  // Delay to give the APC220 time to initialize
    
  // Check if the APC220 responded successfully (optional)
  delay(1000);
    Wire.begin();

}

void loop() {
    
    float sensorUV = (float)analogRead(A1) * 5000 / 1023.0;
    float tempo = millis();
    Serial.print(sensorUV); Serial.print(","); Serial.print(tempo); Serial.print(","); 
    Serial.println("Sending data...");
    delay(1000); // Send data every second
}
