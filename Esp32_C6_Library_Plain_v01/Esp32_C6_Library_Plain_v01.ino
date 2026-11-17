#include "SECRETMaths.h"

// Create an instance of SECRETMaths
SECRETMaths mathTool;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 Library Compiled Plain Code V01");

  int x = 6;
  int y = 7;

  // Test addition
  int additionResult = mathTool.add(x, y);
  Serial.print("Addition: ");
  Serial.print(x);
  Serial.print(" + ");
  Serial.print(y);
  Serial.print(" = ");
  Serial.println(additionResult);

  // Test multiplication
  int multiplicationResult = mathTool.multiply(x, y);
  Serial.print("Multiplication: ");
  Serial.print(x);
  Serial.print(" * ");
  Serial.print(y);
  Serial.print(" = ");
  Serial.println(multiplicationResult);
}

void loop() {
  // Nothing to do here
}
