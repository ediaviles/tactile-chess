const int buttonPin = 13; // set the input pin for the button
int buttonState = 0; // initialize the button state variable

float MIN_BASE_VOLTAGE;
float MAX_BASE_VOLTAGE;
float min_val = -1;
float max_val = -1;

//Keep track of the pieces min and max voltage range when magnetic field is present over HE sensor
struct PieceVoltage {
  String piece;
  float min_voltage;
  float max_voltage;
};

//TODO: REMOVE THIS BOOLEAN
bool TEMP = true;

const char chess_rows[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
const char chess_columns[] = {'1', '2', '3', '4', '5', '6', '7', '8'};

const int MAX_ENTRIES = 15;
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

const int ENABLE_PIN = 2;
const int ROW_SELECT_0 = 4;
const int ROW_SELECT_1 = 5;
const int ROW_SELECT_2 = 6;
const int COL_SELECT_0 = 7;
const int COL_SELECT_1 = 8;
const int COL_SELECT_2 = 9;
const int ANALOG_PIN = A0;

int row = 0;
int col = 0;
int board[8][8];

bool scanned = false; //boolean to see if board has been scanned and callibrated
bool calibration_done = false; //boolean to check if board has been calibrated
bool game_started = false; //boolean to check if board has been calibrated

//const float BASE_VOLTAGE = 2.5; // Base voltage value in Volts

//Function to measure voltage on any given pin
float measure_voltage(int row, int col){
  int colControlPins[] = {COL_SELECT_0, COL_SELECT_1, COL_SELECT_2};
  int rowControlPins[] = {ROW_SELECT_0, ROW_SELECT_1, ROW_SELECT_2};

  int muxChannel[8][3]={
    {0,0,0}, //channel 0
    {1,0,0}, //channel 1
    {0,1,0}, //channel 2
    {1,1,0}, //channel 3
    {0,0,1}, //channel 4
    {1,0,1}, //channel 5
    {0,1,1}, //channel 6
    {1,1,1}, //channel 7
  };

  //loop through the 3 sig
  for(int i = 0; i < 3; i ++){
    digitalWrite(colControlPins[i], muxChannel[col][i]);
    digitalWrite(rowControlPins[i], muxChannel[row][i]);
  }
  //read the value at the Z pin
  int val = analogRead(ANALOG_PIN);

  //return the value
  float voltage = (val * 5.0) / 1024.0;
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
      return VOLTAGE_RANGES[i].piece;
    }
  }
  Serial.println("ERROR: No piece found, but voltage change detected");
}

//Function to ensure that the board is calibrated
void calibration_check(){
  Serial.println("Calibration in progress... ");
  scanned = false;
  calibration_done = false;
  game_started = false;
  //calibration_check
  if(!scanned){
    for(int r = 0; r < 8; r++){
      for(int c = 0; c < 8; c++){
        float value = measure_voltage(r,c);
        // Serial.println(value);
        if(min_val == -1 || min_val > value){
          min_val = value;
        }
        if(max_val == -1 || max_val < value){
          max_val = value;
        }
        scannedMatrix[r][c] = value;
        //TODO: REMOVE THE FOLLOWING LINE
        prevValues[r][c] = value;
        delay(10);
      }
    }
    scanned = true;
    calibration_done = true;
    MIN_BASE_VOLTAGE = min_val - 0.01;
    MAX_BASE_VOLTAGE = max_val + 0.01;
    Serial.print("Min Voltage: ");
    Serial.println(MIN_BASE_VOLTAGE);
    Serial.print("Max Voltage: ");
    Serial.println(MAX_BASE_VOLTAGE);
    Serial.print("Calibration Complete:");
    delay(1000);
  }
}

void set_chess_board(){
  for(int r = 0; r < 8; r++){
    for(int c = 0; c < 8; c++){
      prevValues[r][c] = measure_voltage(r,c);
      delay(5);
    }
  }
}

void piece_detection(){
  //Loop through and read all 8 values
  for(int r = 0; r < 8; r++){
    for(int c = 0; c < 8; c++){
      float value = measure_voltage(r,c);
      if(prevValues[r][c] == -1 || (prevValues[r][c] != value && (value > MAX_BASE_VOLTAGE || value < MIN_BASE_VOLTAGE))){
        Serial.println(prevValues[r][c]);
        Serial.println(value);
        Serial.println("Change detected...");
        delay(1000);
        //STABILIZING LOGIC
          // float lastValue = -1;
          // int counter = 0;
          // bool found = false;
          // while(true){
          //   value = readMux(r,c);
          //   if(lastValue == -1 || lastValue == value){
          //     counter++;
          //     if(counter == 4){
          //       found = true;
          //       break;
          //     }
          //     lastValue = value;
          //   }
          //   break;
          // }
        value = measure_voltage(r,c);
        prevValues[r][c] = value;
        // String piece = find_piece_type(value);
        Serial.print("Piece information: ");
        // Serial.print(piece);
        Serial.print(chess_rows[r]);
        Serial.println(chess_columns[c]);
        delay(10);
      }
    }
  }
}

void setup() {
  pinMode(buttonPin, INPUT); // set the button pin as input
  TEMP = true;
  //MUX Pins
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(COL_SELECT_2, OUTPUT);
  pinMode(ROW_SELECT_0, OUTPUT);
  pinMode(ROW_SELECT_1, OUTPUT);
  pinMode(ROW_SELECT_2, OUTPUT);
  pinMode(COL_SELECT_0, OUTPUT);
  pinMode(COL_SELECT_1, OUTPUT);
  pinMode(ANALOG_PIN, INPUT); //Define Analog Pin
  
  Serial.begin(9600); // Initialize serial communication
  // Keep digital pin 7 always high
  digitalWrite(ENABLE_PIN, HIGH);

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
  for(int i = 0; i<8; i++){
    for(int k = 0;  k<8;k++){
      prevValues[i][k] = -1;
    }
  }
  Serial.flush();
}

void loop() {
  buffer = Serial.readStringUntil('\n'); //Sends 'Start Calibration' message over
  Serial.println(buffer);
  // Serial.println(TEMP);
  if(buffer == "Start Calibration"){
    // Serial.println("Calibration...");
    calibration_check();
    TEMP = false;
  }

  //TODO: ADD LOGIC TO WAIT FOR PIECES TO BE PLACED ON THE BOARD
  if(calibration_done && !game_started){
    set_chess_board();
    game_started = true;
    Serial.println("Game Started");
  }
  
  //piece_detection
  else if (game_started){
    //If game has started then mux logic should constantly run and check for change in voltages
    piece_detection();
    delay(5);
  }
  Serial.flush();
}
