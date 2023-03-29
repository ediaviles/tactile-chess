void setup() {
  Serial.begin(9600); // set up serial communication at 9600 baud rate
}

void loop() {
  int analogValue1 = analogRead(A0); // read analog input A0
  float voltage1 = analogValue1 * (5.0 / 1023.0); // convert analog value to voltage
  Serial.print("Voltage1: ");
  Serial.println(voltage1, 2);
  int analogValue2 = analogRead(A1); // read analog input A0
  float voltage2 = analogValue2 * (5.0 / 1023.0); // convert analog value to voltage
  Serial.print("Voltage2: ");
  Serial.println(voltage2, 2); // print voltage with 3 decimal places
  delay(1000); // wait for 1 second before taking the next reading
}
