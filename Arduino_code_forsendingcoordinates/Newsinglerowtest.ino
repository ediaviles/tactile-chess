int binaryValue = 0;
void setup() {
  // Set pin modes for digital output
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(A0,INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  // Set pin 2 high
  digitalWrite(2, HIGH);
  
  // Start serial communication for debugging purposes
  Serial.begin(9600);
}

void loop() {
  // Read analog input on pin A0
  int analogValue = analogRead(A0);
  
  // Convert analog value to voltage
  float voltage = (analogValue * 5.0) / 1023.0;
  
  // Output voltage value to serial monitor for debugging purposes
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("Tile: \d",binaryValue);
  
  // Increment binary value of pins 3, 4, and 5 until it equals 7
  
    digitalWrite(3, bitRead(binaryValue, 0));
    digitalWrite(4, bitRead(binaryValue, 1));
    digitalWrite(5, bitRead(binaryValue, 2));
    binaryValue++;
    if(binaryValue >= 7){
      binaryValue = 0;
    }
    delay(500); // Delay for half a second to display each binary count
}