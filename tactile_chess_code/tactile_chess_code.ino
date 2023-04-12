const int buttonPin = 13; // set the input pin for the button
int buttonState = 0; // initialize the button state variable

int sensorPin1 = A0;
int sensorPin2 = A1;
int sensorPin3 = A2;

float BASE_VOLTAGES[2];

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

  for(int i = 0; i < 2; i++){
    BASE_VOLTAGES[i] = -1;
  }
  
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
      BASE_VOLTAGES[0] = min_voltage - 0.01;
      BASE_VOLTAGES[1] = max_voltage + 0.01;
      Serial.print("Min Voltage: ");
      Serial.println(BASE_VOLTAGES[0]);
      Serial.print("Max Voltage: ");
      Serial.println(BASE_VOLTAGES[1]);
      Serial.print("Calibration Complete:"); // print a message to the serial monitor
      Serial.print(BASE_VOLTAGES[0]);
      Serial.print(" ");
      Serial.print(BASE_VOLTAGES[1]);
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

  else if(game_started){
    delay(1000);
    Serial.println("pe2");
    delay(2000); 
    Serial.println("pe4");
    delay(1000);
  }
  //color_detection
  else if (game_started && false){
    for (int i = 0; i < 3; i++){
      float voltage = measure_voltage(demo[i]); // Measure voltage using the function from the voltage_measurement file
      demo_volt[i] = voltage;
      if(voltage > BASE_VOLTAGES[1] || voltage < BASE_VOLTAGES[0]){
        Serial.print("Voltage on pin ");
        Serial.print(demo[i]);
        Serial.print(": ");
        Serial.print(demo_volt[i]); // Print the voltage to 2 decimal places
        Serial.println("V");

        if (voltage > BASE_VOLTAGES[1]) {
          Serial.println("White piece detected");
        } else if (voltage < BASE_VOLTAGES[0]) {
          Serial.println("Black piece detected");
        } else {
          Serial.println("no piece detected");
        }
        
        delay(1000); // debounce the button
      }
    }
  }
}
