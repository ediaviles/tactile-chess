void setup() {
  // put your setup code here, to run once:
    pinMode(A0, INPUT);
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue1 = analogRead(A0); // read analog input A0
  float voltage1 = analogValue1 * (5.0 / 1023.0); // convert analog value to voltage
  Serial.print("Voltage1: ");
  Serial.println(voltage1, 2);
  delay(1000);
}
