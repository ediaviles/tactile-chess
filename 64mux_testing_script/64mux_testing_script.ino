 /* 
Modified on Dec 1, 2020
Modified by MehranMaleki from Arduino Examples
Home
*/


//Mux row select pins
int r0 = 4;
int r1 = 5;
int r2 = 6;

//Mux col select pins
int c0 = 8;
int c1 = 9;
int c2 = 10;

//General enablepin
int en = 2;

//Mux in "Z" pin
int Z_pin = 0;


void setup(){
  pinMode(r0, OUTPUT); 
  pinMode(r1, OUTPUT); 
  pinMode(r2, OUTPUT); 

  pinMode(c0, OUTPUT);
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  
  pinMode(en, OUTPUT);

  digitalWrite(r0, LOW);
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  digitalWrite(c0, LOW);
  digitalWrite(c1, LOW);
  digitalWrite(c2, LOW);
  digitalWrite(en, HIGH);

  Serial.begin(9600);
}


void loop(){

  //Loop through and read all 8 values
  for(int r = 5; r < 6; r++){
    for(int c = 6; c < 7; c++){
      Serial.print("Value at r");
      Serial.print(r);
      Serial.print(" c");
      Serial.print(c);
      Serial.print(" is : ");
      Serial.println(readMux(r, c));
      delay(100);
    }
  }
}




float readMux(int row, int col){
  int colControlPins[] = {c0, c1, c2};
  int rowControlPins[] = {r0, r1, r2};

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
  int val = analogRead(Z_pin);

  //return the value
  float voltage = (val * 5.0) / 1024.0;
  return voltage;
}
