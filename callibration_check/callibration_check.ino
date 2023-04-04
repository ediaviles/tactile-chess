int row = 0;
int col = 0;
int board[8][8];

void setup() {
  bool scanned = false;
  pinMode(DIGITAL_PIN_1, OUTPUT);
  pinMode(DIGITAL_PIN_2, OUTPUT);
  pinMode(DIGITAL_PIN_3, OUTPUT);
  pinMode(DIGITAL_PIN_4, OUTPUT);
  pinMode(DIGITAL_PIN_5, OUTPUT);
  pinMode(DIGITAL_PIN_6, OUTPUT);
  pinMode(DIGITAL_PIN_7, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
  Serial.begin(115200);
  
  // Keep digital pin 7 always high
  digitalWrite(DIGITAL_PIN_7, HIGH);
}

void loop() {
  if(scanned){
    // Count from 0 to 7 in binary
    digitalWrite(DIGITAL_PIN_1, bitRead(row, 0));
    digitalWrite(DIGITAL_PIN_2, bitRead(row, 1));
    digitalWrite(DIGITAL_PIN_3, bitRead(row, 2));
    
    
    // Count from 0 to 7 in a group of 3
    digitalWrite(DIGITAL_PIN_4, bitRead(col, 0));
    digitalWrite(DIGITAL_PIN_5, bitRead(col, 1));
    digitalWrite(DIGITAL_PIN_6, bitRead(col, 2));
  
    int currValue = analogRead(ANALOG_PIN);
    float currVoltage = currValue * (5.0 / 1023.0); // convert analog value to voltage
    
    matrix[row][col] = currVoltage;
  
    // Increment counters
    row++;
    if (row == 8) {
      row = 0;
      col++;
      if (col == 8) {
        scanned = true;
        col = 0;
      }
    }
  }
  

  else{ // Get the range of voltage readings for no magnets and block of values in that range
    int min_voltage = -1;
    int max_voltage = -1;

    for (int r = 0; r < 8; r++){
      for (int c = 0; c < 8; c++){
        voltage = matrix[r][c]
        if(min_voltage == -1 || max_voltage == -1){
          min_voltage = voltage;
          max_voltage = voltage;
        } else if (voltage < min_voltage){
          min_voltage = voltage;
        } else if (voltage > max_voltage){
          max_voltage = voltage;
        }
      }
    }
    Serial.print("Callibration in progress... ");
    Serial.print(min_voltage);
    Serial.print(" ");
    Serial.print(max_voltage);
    Serial.println();
  }
}
