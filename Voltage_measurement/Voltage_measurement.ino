float measure_voltage() {
  int analogValue1 = analogRead(A0); // read analog input A0
  float voltage1 = analogValue1 * (5.0 / 1023.0); // convert analog value to voltage
  Serial.print("Voltage1: ");
  Serial.println(voltage1, 2);
  
  int analogValue2 = analogRead(A1); // read analog input A1
  float voltage2 = analogValue2 * (5.0 / 1023.0); // convert analog value to voltage
  Serial.print("Voltage2: ");
  Serial.println(voltage2, 2); // print voltage with 3 decimal places

  int analogValue3 = analogRead(A2); // read analog input A2
  float voltage3 = analogValue3 * (5.0 / 1023.0); // convert analog value to voltage
  Serial.print("Voltage3: ");
  Serial.println(voltage3, 2); // print voltage with 3 decimal places
}
