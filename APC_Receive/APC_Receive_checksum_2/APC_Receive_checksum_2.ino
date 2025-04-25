const int NUM_VALUES = 9;
float receivedValues[NUM_VALUES];
byte receivedChecksum;

void setup() {
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  static String inputLine = "";

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      // End of line: parse the line
      parseLine(inputLine);
      inputLine = "";  // Clear buffer for next line
    } else {
      inputLine += c;
    }
  }
}

void parseLine(String line) {
  int index = 0;
  int lastComma = -1;

  for (int i = 0; i < NUM_VALUES + 1; i++) {
    int nextComma = line.indexOf(',', lastComma + 1);

    if (nextComma == -1) nextComma = line.length();

    String valueStr = line.substring(lastComma + 1, nextComma);
    float value = valueStr.toFloat();

    if (i < NUM_VALUES) {
      receivedValues[i] = value;
    } else {
      receivedChecksum = (byte)value;
    }

    lastComma = nextComma;
  }

  // Recalculate checksum
  unsigned long sum = 0;
  for (int i = 0; i < NUM_VALUES; i++) {
    sum += (unsigned long)(receivedValues[i] * 100);
  }
  byte calculatedChecksum = sum % 256;

  if (calculatedChecksum == receivedChecksum) {
    // Print values
    for (int i = 0; i < NUM_VALUES; i++) {
      Serial.print(receivedValues[i], 2);
      if (i < NUM_VALUES - 1) Serial.print(",");
    }
    Serial.println();
  } else {
    Serial.println("Checksum mismatch.");
  }
}