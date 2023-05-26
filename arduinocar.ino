const int senzorV = A0;  

void setup() {
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  int val = analogRead(senzorV);  // Read sensor value
  float tensiune = val * (5.0 / 1023.0) * (25.0 / 5.0);  
  Serial.print(tensiune, 2); 
  Serial.println("V");

  delay(1000);
}
