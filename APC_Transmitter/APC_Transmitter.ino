/*This script was integrated in the main CanSat arduino
 code to transmit the data through the APC220*/

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
    float time = millis();
    Serial.print(sensorUV); Serial.print(","); Serial.print(time); 
    Serial.println(); 
    delay(1000); // Send data every second
}
