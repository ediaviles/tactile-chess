char pieces[] = {'p', 'r', 'n', 'b', 'q', 'k', 'P', 'R', 'N', 'B', 'Q', 'K'};
char files[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
char ranks[] = {'1', '2', '3', '4', '5', '6', '7', '8'};

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // Seed the random number generator
}

void loop() {
  char result[4];
  result[0] = pieces[random(12)]; // Choose a random piece
  result[1] = files[random(8)]; // Choose a random file
  result[2] = ranks[random(8)]; // Choose a random rank
  result[3] = '\0'; // Null-terminate the string
  
  Serial.println("Calibration Complete"); // Print the result to the serial monitor
  
  delay(1000); // Wait for a second before generating another combination
  
}
