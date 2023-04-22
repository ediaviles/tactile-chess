 /* 
Modified on Dec 1, 2020
Modified by MehranMaleki from Arduino Examples
Home
*/


//Mux control pins
int s0 = 4;
int s1 = 5;
int s2 = 6;
int en = 2;

//Mux in "Z" pin
int Z_pin = 0;


void setup(){
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(en, OUTPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(en, HIGH);

  Serial.begin(9600);
}


void loop(){

  //Loop through and read all 8 values
  for(int i = 7; i > -1; i --){
    Serial.print("Value at channel ");
    Serial.print(i);
    Serial.print(" is : ");
    Serial.println(readMux(i));
    delay(1000);
  }

}




float readMux(int channel){
  int controlPin[] = {s0, s1, s2};

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
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  //read the value at the Z pin
  int val = analogRead(Z_pin);

  //return the value
  float voltage = (val * 5.0) / 1024.0;
  return voltage;
}
