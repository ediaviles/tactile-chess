float BASE_MIN_VOLTAGE = 2.47;
float BASE_MAX_VOLTAGE = 2.51;

struct PieceVoltage {
  String piece;
  float min_voltage;
  float max_voltage;
};

const int MAX_ENTRIES = 10;
PieceVoltage VOLTAGE_RANGES[MAX_ENTRIES];


void setup(){
  
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);

    //Add voltage range for each unique piece 
    //TODO: MAKE THE VALUES THE DIFFERENCE INSTEAD? NOT SURE    
    VOLTAGE_RANGES[0] = {"p", 2.31, 2.35};
    VOLTAGE_RANGES[1] = {"P", 2.63, 2.67};
    VOLTAGE_RANGES[2] = {"r", 2.21, 2.25};
    VOLTAGE_RANGES[3] = {"R", 2.73, 2.77};
    VOLTAGE_RANGES[4] = {"n", 2.15, 2.19};
    VOLTAGE_RANGES[5] = {"N", 2.81, 2.85};
    
    Serial.begin(9600);
}

void loop(){
  int analogValue1 = analogRead(A0); // read analog input A0
  float voltage1 = analogValue1 * (5.0 / 1023.0); // convert analog value to voltage
  Serial.print("Voltage1: ");
  Serial.println(voltage1, 2);
  if(voltage1 < BASE_MIN_VOLTAGE || voltage1 > BASE_MAX_VOLTAGE){
    for (int i = 0; i < MAX_ENTRIES; i++) {
      float min_voltage = VOLTAGE_RANGES[i].min_voltage;
      float max_voltage = VOLTAGE_RANGES[i].max_voltage;
      if(voltage1 >= min_voltage && voltage1 <= max_voltage){
        Serial.print("Piece is:");
        Serial.println(VOLTAGE_RANGES[i].piece);
        break;
      }
      
    }
  }
  
  delay(1000);
//  
//  int analogValue2 = analogRead(A1); // read analog input A1
//  float voltage2 = analogValue2 * (5.0 / 1023.0); // convert analog value to voltage
//  Serial.print("Voltage2: ");
//  Serial.println(voltage2, 2); // print voltage with 3 decimal places
//
//  int analogValue3 = analogRead(A2); // read analog input A2
//  float voltage3 = analogValue3 * (5.0 / 1023.0); // convert analog value to voltage
//  Serial.print("Voltage3: ");
//  Serial.println(voltage3, 2); // print voltage with 3 decimal places
}
