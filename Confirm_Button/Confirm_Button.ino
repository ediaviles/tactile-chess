const int buttonPin = 2; // set the input pin for the button
int buttonState = 0; // initialize the button state variable

void setup() {
  pinMode(buttonPin, INPUT); // set the button pin as input
  Serial.begin(115200); // initialize serial communication at 9600 bits per second
}

void loop() {
  buttonState = digitalRead(buttonPin); // read the state of the button
  if (buttonState == HIGH) { // if the button is pressed
    Serial.println("Calibration Complete"); // print a message to the serial monitor
    delay(1000); // debounce the button
  }
}
