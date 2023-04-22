const int buttonPin = 13; // set the input pin for the button
int buttonState = 0; // initialize the button state variable

int sensorPin1 = A0;
int sensorPin2 = A1;
int sensorPin3 = A2;

float MIN_BASE_VOLTAGE;
float MAX_BASE_VOLTAGE;

//Keep track of the pieces min and max voltage range when magnetic field is present over HE sensor
struct PieceVoltage {
  String piece;
  float min_voltage;
  float max_voltage;
};

const int MAX_ENTRIES = 10;
PieceVoltage VOLTAGE_RANGES[MAX_ENTRIES];

int demo[3]; //sensors used for interim demo
float demo_volt[3]; //voltage readings for interim demo sensors

const byte BUFFER_SIZE = 50;
String buffer;

//MUX logic variables
int binaryCounter = 0;
int groupCounter = 0;
float matrix[8][8];
float prevValues[8][8];
float scannedMatrix[8][8];

const int DIGITAL_PIN_1 = 2;
const int DIGITAL_PIN_2 = 3;
const int DIGITAL_PIN_3 = 4;
const int DIGITAL_PIN_4 = 5;
const int DIGITAL_PIN_5 = 6;
const int DIGITAL_PIN_6 = 7;
const int DIGITAL_PIN_7 = 8;
const int ANALOG_PIN = A0;

int row = 0;
int col = 0;
int board[8][8];

bool scanned = false; //boolean to see if board has been scanned and callibrated
bool calibration_done = false; //boolean to check if board has been calibrated
bool game_started = false; //boolean to check if board has been calibrated

//const float BASE_VOLTAGE = 2.5; // Base voltage value in Volts

//Function to measure voltage on any given pin
float measure_voltage(int pin) {
  int analogValue = analogRead(pin); // read analog input pin
  float voltage = analogValue * (5.0 / 1023.0); // convert analog value to voltage
  return voltage;
}

//Function to find the piece type depending on the voltage read
void find_piece_type(float voltage) {
  for (int i = 0; i < MAX_ENTRIES; i++) {
    float min_voltage = VOLTAGE_RANGES[i].min_voltage;
    float max_voltage = VOLTAGE_RANGES[i].max_voltage;
    if(voltage >= min_voltage && voltage <= max_voltage){
      Serial.print("Piece is:");
      Serial.println(VOLTAGE_RANGES[i].piece);
      return;
    }
  }
  Serial.println("ERROR: No piece found, but voltage change detected")
}

//Function to ensure that the board is calibrated
void calibration_check(){
  float voltage1 = 0;
  float voltage2 = 0;
  float voltage3 = 0;
  float min_voltage = 0;
  float max_voltage = 0;
  scanned = false;
  calibration_done = false;
  game_started = false;
  //calibration_check
  if(!scanned){
    Serial.println("Scanned: ");
    Serial.println(scanned);
    
    Serial.println("Calibration in progress... ");
    voltage1 = measure_voltage(sensorPin1);
    voltage2 = measure_voltage(sensorPin2);
    voltage3 = measure_voltage(sensorPin3);
    Serial.print("Voltage1: ");
    Serial.println(voltage1);
    Serial.print("Voltage2: ");
    Serial.println(voltage2);
    Serial.print("Voltage3: ");
    Serial.println(voltage3);
    scanned = true;
    calibration_done = true;
    min_voltage = voltage1;
    max_voltage = voltage1;
    if(voltage2 < min_voltage){
      min_voltage = voltage2;
    }
    if(voltage3 < min_voltage){
      min_voltage = voltage3;
    }
    if(voltage2 > max_voltage){
      max_voltage = voltage2;
    }
    if(voltage3 > max_voltage){
      max_voltage = voltage3;
    }
    MIN_BASE_VOLTAGE = min_voltage - 0.01;
    MAX_BASE_VOLTAGE = max_voltage + 0.01;
    Serial.print("Min Voltage: ");
    Serial.println(MIN_BASE_VOLTAGE);
    Serial.print("Max Voltage: ");
    Serial.println(MAX_BASE_VOLTAGE);
    Serial.print("Calibration Complete:"); // print a message to the serial monitor
    Serial.print(MIN_BASE_VOLTAGE);
    Serial.print(" ");
    Serial.print(MAX_BASE_VOLTAGE);
    Serial.println();
    delay(1000);
  }
}

//Logic to control the select lines of the MUXes
void MUX_logic(){
  // Count from 0 to 7 in binary
  digitalWrite(DIGITAL_PIN_1, bitRead(binaryCounter, 0));
  digitalWrite(DIGITAL_PIN_2, bitRead(binaryCounter, 1));
  digitalWrite(DIGITAL_PIN_3, bitRead(binaryCounter, 2));


  // Count from 0 to 7 in a group of 3
  digitalWrite(DIGITAL_PIN_4, bitRead(groupCounter, 0));
  digitalWrite(DIGITAL_PIN_5, bitRead(groupCounter, 1));
  digitalWrite(DIGITAL_PIN_6, bitRead(groupCounter, 2));
  
  int rawValue = analogRead(ANALOG_PIN);
  float currentValue = rawValue * (5.0/1023);
  int digitalValue = (bitRead(binaryCounter, 0) << 0) |
                      (bitRead(binaryCounter, 1) << 1) |
                      (bitRead(binaryCounter, 2) << 2) |
                      (bitRead(groupCounter, 0) << 3) |
                      (bitRead(groupCounter, 1) << 4) |
                      (bitRead(groupCounter, 2) << 5);
  if (currentValue != prevValues[binaryCounter][groupCounter]){
    matrix[binaryCounter][groupCounter] = currentValue;
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
  }else{
    matrix[binaryCounter][groupCounter] = prevValues[binaryCounter][groupCounter];
  }
  // Increment counters
  binaryCounter++;
  if(binaryCounter == 8) {
    binaryCounter = 0;
    groupCounter++;
    if (groupCounter == 8) {
      binaryCounter = 0;
      for(int r = 0;r<8;r++){
        for(int c =0;c<8;c++){
          prevValues[r][c] = matrix[r][c];
        }
      }
    }
  }
}

void setup() {
  pinMode(buttonPin, INPUT); // set the button pin as input

  //MUX Pins
  pinMode(DIGITAL_PIN_1, OUTPUT);
  pinMode(DIGITAL_PIN_2, OUTPUT);
  pinMode(DIGITAL_PIN_3, OUTPUT);
  pinMode(DIGITAL_PIN_4, OUTPUT);
  pinMode(DIGITAL_PIN_5, OUTPUT);
  pinMode(DIGITAL_PIN_6, OUTPUT);
  pinMode(DIGITAL_PIN_7, OUTPUT);
  pinMode(ANALOG_PIN, INPUT); //Define Analog Pin
  
  Serial.begin(9600); // Initialize serial communication
  
  // Keep digital pin 7 always high
  digitalWrite(DIGITAL_PIN_7, HIGH);

  demo[0] = sensorPin1;
  demo[1] = sensorPin2;
  demo[2] = sensorPin3;

  MIN_BASE_VOLTAGE = -1.0f;
  MAX_BASE_VOLTAGE = -1.0f;

  //Add voltage range for each unique piece 
  //TODO: MAKE THE VALUES THE DIFFERENCE INSTEAD? NOT SURE    
  VOLTAGE_RANGES[0] = {"p", 2.39, 2.41};
  VOLTAGE_RANGES[1] = {"P", 2.56, 2.57};
  VOLTAGE_RANGES[2] = {"r", 2.33, 2.35};
  VOLTAGE_RANGES[3] = {"R", 2.63, 2.65};
  VOLTAGE_RANGES[4] = {"n", 2.27, 2.31};
  VOLTAGE_RANGES[5] = {"N", 2.67, 2.69};
  VOLTAGE_RANGES[6] = {"b", 2.24, 2.26};
  VOLTAGE_RANGES[7] = {"B", 2.74, 2.77};
  VOLTAGE_RANGES[8] = {"q", 2.20, 2.23};
  VOLTAGE_RANGES[9] = {"Q", 2.78, 2.81};    
  VOLTAGE_RANGES[10] = {"k", 2.16, 2.18};
  VOLTAGE_RANGES[11] = {"K", 2.82, 2.85};


  for(int i = 0; i < 3; i++){
    demo_volt[i] = -1;
  }
  
  for(int i = 0; i<8; i++){
    for(int k = 0;  k<8;k++){
      prevValues[i][k] = -1;
    }
  }
}

void loop() {
  buffer = Serial.readStringUntil('\n'); //Sends 'Start Calibration' message over
  Serial.println(buffer);
  if(buffer == "Start Calibration"){
    calibration_check();
  }

  if(calibration_done && !game_started){
    Serial.println("Begin Game");
    game_started = true;
  }
  
  //piece_detection
  else if (game_started){
    //If game has started then mux logic should constantly run and check for change in voltages
    //TODO: ADD MUX LOGIC HERE (STORE THE VOLTAGE IN A VARIABLE TO FIND PIECE)
    
    //Find piece
    //TODO: UNCOMMENT THE BOTTOM 2 lines
    //String piece find_piece_type(voltage);

    //First identify type of piece
    for (int i = 0; i < 3; i++){
      float voltage = measure_voltage(demo[i]); // Measure voltage using the function from the voltage_measurement file
      demo_volt[i] = voltage;
      if(voltage > MIN_BASE_VOLTAGE || voltage < MAX_BASE_VOLTAGE){
        Serial.print("Voltage on pin ");
        Serial.print(demo[i]);
        Serial.print(": ");
        Serial.print(demo_volt[i]); // Print the voltage to 2 decimal places
        Serial.println("V");
        delay(1000);
      }
      else{
        find_piece_type(voltage);
      }
    }
    //Second identify the coordinate    
    
  }
}
