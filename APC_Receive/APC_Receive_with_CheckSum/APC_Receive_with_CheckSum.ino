const int NUM_VALUES = 9;               // You expect 9 values
uint16_t receivedValues[NUM_VALUES];    // A place to store the received numbers
byte receivedChecksum;                  // A variable to store the checksum

void setup() {
  Serial.begin(9600);                   // Start serial communication
  delay(2000);                          // Wait a moment for the radio to initialize
}

void loop() {
  // Wait until enough data has arrived: 9 values * 2 bytes + 1 checksum byte
  if (Serial.available() >= NUM_VALUES * 2 + 1) {
    
    // Step 1: Read the 9 values (each value is 2 bytes: low + high)
    for (int i = 0; i < NUM_VALUES; i++) {
      byte lowByte = Serial.read();                      // Read the lower byte (first half)
      byte highByte = Serial.read();                     // Read the upper byte (second half)
      receivedValues[i] = (highByte << 8) | lowByte;     // Combine to get the original value
    }

    // Step 2: Read the last byte as the checksum
    receivedChecksum = Serial.read();

    // Step 3: Recalculate the checksum from the received values
    unsigned long sum = 0;
    for (int i = 0; i < NUM_VALUES; i++) {
      sum += receivedValues[i];  // Just add them all up
    }
    byte calculatedChecksum = sum % 256;  // Only keep the last 1 byte of the result

    // Step 4: Compare the received checksum with the calculated one
    if (calculatedChecksum == receivedChecksum) {
      // If they match, the data is valid — now print it
      for (int i = 0; i < NUM_VALUES; i++) {
        float value = receivedValues[i] / 100.0;    // Convert back to float (undo the *100 from sender)
        Serial.print(value, 2);                     // Print with 2 decimal digits
        if (i < NUM_VALUES - 1) {
          Serial.print(",");                        // Add comma between numbers (but not after the last one)
        }
      }
      Serial.println();  // Add a newline after the line is printed
    }

    // If the checksum does not match, do nothing (just ignore bad data)
  }
}


// Code developed with assistance from ChatGPT (OpenAI), April 2025
