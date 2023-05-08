#include <stdlib.h>

float MIN_BASE_VOLTAGE;
float MAX_BASE_VOLTAGE;

//Keep track of the pieces min and max voltage range when magnetic field is present over HE sensor
struct PieceVoltage {
  String piece;
  float min_voltage;
  float max_voltage;
};

struct SensorInfo {
  float BASE_VOLTAGE;
  float DELTA;
};

const float DELTA = 0.07;

const char chess_columns[] = {'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};
const char chess_rows[] = {'1', '2', '3', '4', '5', '6', '7', '8'};

const int MAX_ENTRIES = 15;
PieceVoltage VOLTAGE_RANGES[MAX_ENTRIES];

SensorInfo SENSOR_BOARD[8][8];

float prevValues[8][8];

const int ENABLE_PIN = 2;
const int ROW_SELECT_0 = 4;
const int ROW_SELECT_1 = 5;
const int ROW_SELECT_2 = 6;
const int COL_SELECT_0 = 8;
const int COL_SELECT_1 = 9;
const int COL_SELECT_2 = 10;
const int ANALOG_PIN = A0;

//String FEN[8][8];

const byte BUFFER_SIZE = 50;
String buffer;
bool scanned = false; //boolean to see if board has been scanned and callibrated
bool calibration_done = false; //boolean to check if board has been calibrated
bool game_started = false; //boolean to check if board has been calibrated


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
String find_piece_type(float voltage, int r, int c) {
  for (int i = 0; i < MAX_ENTRIES; i++) {
    float min_voltage = VOLTAGE_RANGES[i].min_voltage;
    float max_voltage = VOLTAGE_RANGES[i].max_voltage;
    SensorInfo HE_sensor = SENSOR_BOARD[r][c];
    if((voltage + HE_sensor.DELTA) >= (min_voltage) && (voltage + HE_sensor.DELTA) <= (max_voltage)){
      return VOLTAGE_RANGES[i].piece;
    }
  }
  return "X";
//  Serial.println("ERROR: No piece found, but voltage change detected");
}

//Function to ensure that the board is calibrated
void calibration_check(){
  Serial.println("Calibration in progress... ");
  scanned = false;
  calibration_done = false;
  game_started = false;
  float min_val = -1;
  float max_val = -1;
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
        SENSOR_BOARD[r][c] = {2.48, -10.0};
        delay(10);
      }
    }
    scanned = true;
    calibration_done = true;
    //MIN_BASE_VOLTAGE = min_val;
    //MAX_BASE_VOLTAGE = max_val;
//
    MIN_BASE_VOLTAGE = 2.45;
    MAX_BASE_VOLTAGE = 2.53;
    float BASE_VOLT = (MIN_BASE_VOLTAGE + MAX_BASE_VOLTAGE) / 2;
    Serial.println(BASE_VOLT);
    for (int r = 0; r < 8; r++){
      for (int c = 0; c < 8; c++){
        SENSOR_BOARD[r][c] = {SENSOR_BOARD[r][c].BASE_VOLTAGE, (BASE_VOLT - SENSOR_BOARD[r][c].BASE_VOLTAGE)};
        delay(10);
      }
    }
    Serial.print("Min Voltage: ");
    Serial.println(MIN_BASE_VOLTAGE);
    Serial.print("Max Voltage: ");
    Serial.println(MAX_BASE_VOLTAGE);
    Serial.println("Calibration Complete:");
    delay(1000);
  }
}

float find_max_voltage(float voltages[], int num){
  float max_val = -1;
  for(int c = 0; c < num; c++){
    if(max_val == -1 || voltages[c] > max_val){
      max_val = voltages[c];
    }
  }
  return max_val;
}

float find_min_voltage(float voltages[], int num){
  float min_val = -1;
  for(int c = 0; c < num; c++){
    if(min_val == -1 || voltages[c] < min_val){
      min_val = voltages[c];
    }
  }
  return min_val;
}

void set_pawn_values(){
  if(prevValues[1][0] < MIN_BASE_VOLTAGE){
    VOLTAGE_RANGES[0].min_voltage = find_min_voltage(prevValues[1], 8);
    VOLTAGE_RANGES[0].max_voltage = find_max_voltage(prevValues[1], 8);
    VOLTAGE_RANGES[1].min_voltage = find_min_voltage(prevValues[6], 8);
    VOLTAGE_RANGES[1].max_voltage = find_max_voltage(prevValues[6], 8);
  }
  else if(prevValues[1][0] > MAX_BASE_VOLTAGE){
    VOLTAGE_RANGES[0].min_voltage = find_min_voltage(prevValues[6], 8);
    VOLTAGE_RANGES[0].max_voltage = find_max_voltage(prevValues[6], 8);
    VOLTAGE_RANGES[1].min_voltage = find_min_voltage(prevValues[1], 8);
    VOLTAGE_RANGES[1].max_voltage = find_max_voltage(prevValues[1], 8);
  }
}

void set_rook_values(){
  float bottom_row[] = {prevValues[0][0], prevValues[0][7]};
  float top_row[] = {prevValues[7][0], prevValues[7][7]};
  if(prevValues[1][0] < MIN_BASE_VOLTAGE){
    VOLTAGE_RANGES[2].min_voltage = find_min_voltage(bottom_row, 2);
    VOLTAGE_RANGES[2].max_voltage = find_max_voltage(bottom_row, 2);
    VOLTAGE_RANGES[3].min_voltage = find_min_voltage(top_row, 2);
    VOLTAGE_RANGES[3].max_voltage = find_max_voltage(top_row, 2);
  }
  else if(prevValues[1][0] > MAX_BASE_VOLTAGE){
    VOLTAGE_RANGES[2].min_voltage = find_min_voltage(top_row, 2);
    VOLTAGE_RANGES[2].max_voltage = find_max_voltage(top_row, 2);
    VOLTAGE_RANGES[3].min_voltage = find_min_voltage(bottom_row, 2);
    VOLTAGE_RANGES[3].max_voltage = find_max_voltage(bottom_row, 2);
  }
}

void set_knight_values(){
  float bottom_row[] = {prevValues[0][1], prevValues[0][6]};
  float top_row[] = {prevValues[7][1], prevValues[7][6]};
  if(prevValues[1][0] < MIN_BASE_VOLTAGE){
    VOLTAGE_RANGES[4].min_voltage = find_min_voltage(bottom_row, 2);
    VOLTAGE_RANGES[4].max_voltage = find_max_voltage(bottom_row, 2);
    VOLTAGE_RANGES[5].min_voltage = find_min_voltage(top_row, 2);
    VOLTAGE_RANGES[5].max_voltage = find_max_voltage(top_row, 2);
  }
  else if(prevValues[1][0] > MAX_BASE_VOLTAGE){
    VOLTAGE_RANGES[4].min_voltage = find_min_voltage(top_row, 2);
    VOLTAGE_RANGES[4].max_voltage = find_max_voltage(top_row, 2);
    VOLTAGE_RANGES[5].min_voltage = find_min_voltage(bottom_row, 2);
    VOLTAGE_RANGES[5].max_voltage = find_max_voltage(bottom_row, 2);
  }
}


void set_bishop_values(){
  float bottom_row[] = {prevValues[0][2], prevValues[0][5]};
  float top_row[] = {prevValues[7][2], prevValues[7][5]};
  if(prevValues[1][0] < MIN_BASE_VOLTAGE){
    VOLTAGE_RANGES[6].min_voltage = find_min_voltage(bottom_row, 2);
    VOLTAGE_RANGES[6].max_voltage = find_max_voltage(bottom_row, 2);
    VOLTAGE_RANGES[7].min_voltage = find_min_voltage(top_row, 2);
    VOLTAGE_RANGES[7].max_voltage = find_max_voltage(top_row, 2);
  }
  else if(prevValues[1][0] > MAX_BASE_VOLTAGE){
    VOLTAGE_RANGES[6].min_voltage = find_min_voltage(top_row, 2);
    VOLTAGE_RANGES[6].max_voltage = find_max_voltage(top_row, 2);
    VOLTAGE_RANGES[7].min_voltage = find_min_voltage(bottom_row, 2);
    VOLTAGE_RANGES[7].max_voltage = find_max_voltage(bottom_row, 2);
  }
}


void set_chess_board(){
  for(int r = 0; r < 8; r++){
    for(int c = 7; c > -1; c--){
      prevValues[r][c] = (measure_voltage(r,c) + SENSOR_BOARD[r][c].DELTA);
      delay(10);
    }
  }
//  
//  set_pawn_values();
//  set_rook_values();
//  set_knight_values();
//  set_bishop_values();
//  
  for(int r = 0; r < 8; r++){
    for(int c = 7; c > -1; c--){
      //FEN[r][c] = find_piece_type(prevValues[r][c], r, c);
      delay(10);
    }
  }
  //set_king_queen_values();
}

void print_matrix(String matrix[][8]){
  for (int i = 7; i > -1; i--) {
    for (int j = 7; j > -1; j--) {
      Serial.print(matrix[i][j]);
      Serial.print("\t");
    }
    Serial.println();
  }
}

void print_float_matrix(float matrix[][8]){
  for (int i = 7; i > -1; i--) {
    for (int j = 7; j > -1; j--) {
      Serial.print(matrix[i][j]);
      Serial.print("\t");
    }
    Serial.println();
  }
}

void polarity_detection(){
  for(int r = 0; r < 8; r++){
    for(int c = 7; c > -1; c--){
      float curr_value = (measure_voltage(r,c) + SENSOR_BOARD[r][c].DELTA);
      
      if(abs(curr_value - prevValues[r][c]) >= DELTA){
        delay(1500);
        curr_value = (measure_voltage(r,c) + SENSOR_BOARD[r][c].DELTA);
        String piece = "p";
        Serial.print("Curr_value: ");
        Serial.println(curr_value);
        Serial.print("Prev Value: ");
        Serial.println(prevValues[r][c]);
        if((curr_value >= MIN_BASE_VOLTAGE && curr_value <= MAX_BASE_VOLTAGE) && (prevValues[r][c] < MIN_BASE_VOLTAGE || prevValues[r][c] > MAX_BASE_VOLTAGE)){
          if(prevValues[r][c] > MAX_BASE_VOLTAGE){
            piece = "P";
          }
        }
        else if((prevValues[r][c] >= MIN_BASE_VOLTAGE && prevValues[r][c] <= MAX_BASE_VOLTAGE) && (curr_value < MIN_BASE_VOLTAGE || curr_value > MAX_BASE_VOLTAGE)){
         
          if(curr_value > MAX_BASE_VOLTAGE){
            piece = "P";
          }
        }
        Serial.print(piece);
        Serial.print(chess_columns[c]);
        Serial.println(chess_rows[r]);
        float real_voltage = measure_voltage(r, c);
        Serial.println(real_voltage);
        Serial.println(real_voltage + SENSOR_BOARD[r][c].DELTA);
        prevValues[r][c] = (real_voltage + SENSOR_BOARD[r][c].DELTA);
        //FEN[r][c] = find_piece_type(prevValues[r][c], r, c);
        //print_float_matrix(prevValues);
        //print_matrix(FEN);
      }
    }
  }
}

//void piece_detection(){
//  //Loop through and read all 8 values
//  for(int r = 0; r < 8; r++){
//    for(int c = 7; c > -1; c--){
//      float curr_value = measure_voltage(r,c);
//      
//      if(abs(curr_value - prevValues[r][c] >= DELTA)){
//        delay(2000);
//        //STABILIZING LOGIC
//          // float lastValue = -1;
//          // int counter = 0;
//          // bool found = false;
//          // while(true){
//          //   value = readMux(r,c);
//          //   if(lastValue == -1 || lastValue == value)
//          //     counter++;
//          //     if(counter == 4){
//          //       found = true;
//          //       break;
//          //     }
//          //     lastValue = value;
//          //   }
//          //   break;
//          // }
//          
//        float stab_val = measure_voltage(r,c);
//        String moved_piece = find_piece_type(stab_val, r, c);
//        if((prevValues[r][c] != stab_val) && (FEN[r][c] != moved_piece)){
//          Serial.println("Change detected...");
//          Serial.print("Value Before Checking = ");
//          Serial.println(curr_value);
//          Serial.print("Previous Value = ");
//          Serial.println(prevValues[r][c]);
//          Serial.print("Current value = ");
//          Serial.println(stab_val);
//          Serial.print("Previous Piece = ");
//          Serial.println(FEN[r][c]);
//          Serial.print("Current Piece (BC) = ");
//          Serial.println(moved_piece);
//          if(FEN[r][c] != "X" && moved_piece == "X"){
//            moved_piece = FEN[r][c];
//          }
//          Serial.print("Current Piece = ");
//          Serial.println(moved_piece);
//          prevValues[r][c] = stab_val;
//          FEN[r][c] = moved_piece;
//          // String piece = find_piece_type(value, r, c);
//          Serial.print("Piece information: ");
//          Serial.print(moved_piece);
//          Serial.print(chess_columns[c]);
//          Serial.println(chess_rows[r]);
//          print_matrix(FEN);
//        } 
//      }
//    }
//  }
//}

void setup() {
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

  MIN_BASE_VOLTAGE = -0.1f;
  MAX_BASE_VOLTAGE = -0.1f;

  //Add voltage range for each unique piece 
  //TODO: MAKE THE VALUES THE DIFFERENCE INSTEAD? NOT SURE    
  VOLTAGE_RANGES[0] = {"p", 2.13, 2.25};
  VOLTAGE_RANGES[2] = {"r", 2.04, 2.12};
  VOLTAGE_RANGES[4] = {"n", 1.82, 1.94};
  VOLTAGE_RANGES[6] = {"b", 1.94, 2.04};
  VOLTAGE_RANGES[8] = {"q", 1.74, 1.82};
  VOLTAGE_RANGES[10] = {"k", 1.50, 1.65};
  
  VOLTAGE_RANGES[1] = {"P", 2.70, 2.85};
  VOLTAGE_RANGES[3] = {"R", 2.87, 3.02};
  VOLTAGE_RANGES[5] = {"N", 3.02, 3.14};
  VOLTAGE_RANGES[7] = {"B", 3.15, 3.27};
  VOLTAGE_RANGES[9] = {"Q", 3.27, 3.39};    
  VOLTAGE_RANGES[11] = {"K", 3.43, 3.53};
 
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
  if(buffer == "Start Calibration"){
    calibration_check();
    //calibration_done = true;
  }
  //TODO: ADD LOGIC TO WAIT FOR PIECES TO BE PLACED ON THE BOAR
  if(buffer == "Begin Game" && (true || calibration_done) && !game_started){
    set_chess_board();
    game_started = true;
    Serial.println("Begin Game");
  }
  
  //piece_detection
  else if (game_started){
    //If game has started then mux logic should constantly run and check for change in voltages
    //piece_detection();
    polarity_detection();
    delay(5);
  }
  Serial.flush();
}
