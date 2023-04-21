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

const int DIGITAL_PIN_1 = 2;
const int DIGITAL_PIN_2 = 3;
const int DIGITAL_PIN_3 = 4;
const int DIGITAL_PIN_4 = 5;
const int DIGITAL_PIN_5 = 6;
const int DIGITAL_PIN_6 = 7;
const int DIGITAL_PIN_7 = 8;

int row = 0;
int col = 0;
int board[8][8];

bool scanned = false; //boolean to see if board has been scanned and callibrated
bool calibration_done = false; //boolean to check if board has been calibrated
bool game_started = false; //boolean to check if board has been calibrated

//const float BASE_VOLTAGE = 2.5; // Base voltage value in Volts

float measure_voltage(int pin) {
  int analogValue = analogRead(pin); // read analog input pin
  float voltage = analogValue * (5.0 / 1023.0); // convert analog value to voltage
  return voltage;
}

void find_piece_type(float voltage) {
for (int i = 0; i < MAX_ENTRIES; i++) {
    float min_voltage = VOLTAGE_RANGES[i].min_voltage;
    float max_voltage = VOLTAGE_RANGES[i].max_voltage;
    if(voltage >= min_voltage && voltage <= max_voltage){
      Serial.print("Piece is:");
      Serial.println(VOLTAGE_RANGES[i].piece);
      break;
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
//  pinMode(ANALOG_/PIN, INPUT); //Define Analog Pin
  
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
  VOLTAGE_RANGES[0] = {"p", 2.31, 2.35};
  VOLTAGE_RANGES[1] = {"P", 2.63, 2.67};
  VOLTAGE_RANGES[2] = {"r", 2.21, 2.25};
  VOLTAGE_RANGES[3] = {"R", 2.73, 2.77};
  VOLTAGE_RANGES[4] = {"n", 2.15, 2.19};
  VOLTAGE_RANGES[5] = {"N", 2.81, 2.85};
  
  for(int i = 0; i < 3; i++){
    demo_volt[i] = -1;
  }
    
}

void loop() {
//  Serial.println("Test"); // print a message to the serial monitor
  float voltage1 = 0;
  float voltage2 = 0;
  float voltage3 = 0;
  float min_voltage = 0;
  float max_voltage = 0;
  buffer = Serial.readStringUntil('\n'); //Sends 'Start Calibration' message over
//  while (Serial.available() > 0) {
//      char incomingByte = Serial.read();
//      buffer += incomingByte;
//    }
    Serial.println(buffer);
//  Serial.println("Tes/t");
  if(buffer == "Start Calibration"){
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


//  else if(buffer == "Wait for Begin Game"){
//    calibration_done = true;
//  }
  
  if(calibration_done && !game_started){
    Serial.println("Begin Game");
    game_started = true;
  }
  
  //color_detection
  else if (game_started){
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

        find_piece_type(voltage);
        
        delay(1000); // debounce the button
      }
    }
    //Second identify the coordinate

//    
    
  }
}
