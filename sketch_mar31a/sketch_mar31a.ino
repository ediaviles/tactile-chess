void setup(){
    Serial.begin(9600);
}

void loop(){
    if (Serial.available()) {
        byte nr = Serial.read();
        Serial.print("The following char was received: ");
        Serial.println(nr, DEC);
    }
}
