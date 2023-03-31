const int DIGITAL_PIN_1 = 2;
const int DIGITAL_PIN_2 = 3;
const int DIGITAL_PIN_3 = 4;
const int DIGITAL_PIN_4 = 5;
const int DIGITAL_PIN_5 = 6;
const int DIGITAL_PIN_6 = 7;
const int DIGITAL_PIN_7 = 8;
const int ANALOG_PIN = A0;

int binaryCounter = 0;
int groupCounter = 0;
int matrix[8][8];
int prevValues[8];

void setup() {
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

  for(int i = 0; i<8; i++){
    prevValues[i] = -1;
  }
}



void loop() {
  // Count from 0 to 7 in binary
  digitalWrite(DIGITAL_PIN_1, bitRead(binaryCounter, 0));
  digitalWrite(DIGITAL_PIN_2, bitRead(binaryCounter, 1));
  digitalWrite(DIGITAL_PIN_3, bitRead(binaryCounter, 2));
  
  
  // Count from 0 to 7 in a group of 3
  digitalWrite(DIGITAL_PIN_4, bitRead(groupCounter, 0));
  digitalWrite(DIGITAL_PIN_5, bitRead(groupCounter, 1));
  digitalWrite(DIGITAL_PIN_6, bitRead(groupCounter, 2));
  
  
  // Save analog value to matrix when a digital pin changes output
  if (digitalRead(DIGITAL_PIN_1) != bitRead(binaryCounter, 0) ||
      digitalRead(DIGITAL_PIN_2) != bitRead(binaryCounter, 1) ||
      digitalRead(DIGITAL_PIN_3) != bitRead(binaryCounter, 2) ||
      digitalRead(DIGITAL_PIN_4) != bitRead(groupCounter, 0) ||
      digitalRead(DIGITAL_PIN_5) != bitRead(groupCounter, 1) ||
      digitalRead(DIGITAL_PIN_6) != bitRead(groupCounter, 2)) {
    int currentValue = analogRead(ANALOG_PIN);
    int digitalValue = (bitRead(binaryCounter, 0) << 0) |
                       (bitRead(binaryCounter, 1) << 1) |
                       (bitRead(binaryCounter, 2) << 2) |
                       (bitRead(groupCounter, 0) << 3) |
                       (bitRead(groupCounter, 1) << 4) |
                       (bitRead(groupCounter, 2) << 5);
    if (currentValue != prevValues[binaryCounter]){
      matrix[binaryCounter][groupCounter] = currentValue;
      prevValues[binaryCounter] = currentValue;
      int combined = (groupCounter << 3) | binaryCounter;
      switch(combined){
        case 0:
          Serial.println("a1");
          break;
        case 1:
          Serial.println("b1");
          break;
        case 2:
          Serial.println("c1");
          break;
        case 3:
          Serial.println("d1");
          break;
        case 4:
          Serial.println("e1");
          break;
        case 5:
          Serial.println("f1");
          break;
        case 6:
          Serial.println("g1");
          break;
        case 7:
          Serial.println("h1");
          break;
        case 8:
          Serial.println("a2");
          break;
        case 9:
          Serial.println("b2");
          break;
        case 10:
          Serial.println("c2");
          break;
        case 11:
          Serial.println("d2");
          break;
        case 12:
          Serial.println("e2");
          break;
        case 13:
          Serial.println("f2");
          break;
        case 14:
          Serial.println("g2");
          break;
        case 15:
          Serial.println("h2");
          break;
        case 16:
          Serial.println("a3");
          break;
        case 17:
          Serial.println("b3");
          break;
        case 18:
          Serial.println("c3");
          break;
        case 19:
          Serial.println("d3");
          break;
        case 20:
          Serial.println("e3");
          break;
        case 21:
          Serial.println("f3");
          break;
        case 22:
          Serial.println("g3");
          break;
        case 23:
          Serial.println("h3");
          break;
        case 24:
          Serial.println("a4");
          break;
        case 25:
          Serial.println("b4");
          break;
        case 26:
          Serial.println("c4");
          break;
        case 27:
          Serial.println("d4");
          break;
        case 28:
          Serial.println("e4");
          break;
        case 29:
          Serial.println("f4");
          break;
        case 30:
          Serial.println("g4");
          break;
        case 31:
          Serial.println("h4");
          break;
        case 32:
          Serial.println("a5");
          break;
        case 33:
          Serial.println("b5");
          break;
        case 34:
          Serial.println("c5");
          break;
        case 35:
          Serial.println("d5");
          break;
        case 36:
          Serial.println("e5");
          break;
        case 37:
          Serial.println("f5");
          break;
        case 38:
          Serial.println("g5");
          break;
        case 39:
          Serial.println("h5");
          break;
        case 40:
          Serial.println("a6");
          break;
        case 41:
          Serial.println("b6");
          break;
        case 42:
          Serial.println("c6");
          break;
        case 43:
          Serial.println("d6");
          break;
        case 44:
          Serial.println("e6");
          break;
        case 45:
          Serial.println("f6");
          break;
        case 46:
          Serial.println("g6");
          break;
        case 47:
          Serial.println("h6");
          break;
        case 48:
          Serial.println("a7");
          break;
        case 49:
          Serial.println("b7");
          break;
        case 50:
          Serial.println("c7");
          break;
        case 51:
          Serial.println("d7");
          break;
        case 52:
          Serial.println("e7");
          break;
        case 53:
          Serial.println("f7");
          break;
        case 54:
          Serial.println("g7");
          break;
        case 55:
          Serial.println("h7");
          break;
        case 56:
          Serial.println("a8");
          break;
        case 57:
          Serial.println("b8");
          break;
        case 58:
          Serial.println("c8");
          break;
        case 59:
          Serial.println("d8");
          break;
        case 60:
          Serial.println("e8");
          break;
        case 61:
          Serial.println("f8");
          break;
        case 62:
          Serial.println("g8");
          break;
        case 63:
          Serial.println("h8");
          break;
      }
      
     }
  }
  
  // Increment counters
  binaryCounter++;
  if (binaryCounter == 8) {
    binaryCounter = 0;
    groupCounter++;
    if (groupCounter == 8) {
      binaryCounter = 0;
    }
  }
  
  // Wait for a short period before updating again
  delay(30);
}