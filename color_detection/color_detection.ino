const float BASE_VOLTAGE = 2.5; // Base voltage value in Volts

float measure_voltage(int pin) {
  int analogValue = analogRead(pin); // read analog input pin
  float voltage = analogValue * (5.0 / 1023.0); // convert analog value to voltage
  return voltage;
}


void setup() {
  Serial.begin(115200); // Initialize serial communication
}

void loop() {
  int inputPin = A0;
  float voltage = measure_voltage(inputPin); // Measure voltage using the function from the voltage_measurement file
  
  Serial.print("Voltage on pin ");
  Serial.print(inputPin);
  Serial.print(": ");
  Serial.print(voltage, 2); // Print the voltage to 2 decimal places
  Serial.println("V");
  
  if (voltage > BASE_VOLTAGE) {
    Serial.println("Voltage is higher than base voltage");
  } else if (voltage < BASE_VOLTAGE) {
    Serial.println("Voltage is lower than base voltage");
  } else {
    Serial.println("Voltage is equal to base voltage");
  }
  
  delay(1000); // Wait for 1 second
}
