void setup() {
  Serial.begin(9600);  // Start serial communication with PC
  delay(2000);  // Delay to give the APC220 time to initialize
  
}

void loop() {
  if (Serial.available()) {  // Check if data is coming from APC220
    char incomingByte = Serial.read();  // Read incoming byte
    Serial.print(incomingByte);  // Print received data
  }
}
