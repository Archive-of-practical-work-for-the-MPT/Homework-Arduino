void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  float joyX = (analogRead(A0) - 512) / 512.0;
  float joyY = (analogRead(A1) - 512) / 512.0;
  
  // Сглаживание значений
  joyX = 0.8 * joyX + 0.2 * ((analogRead(A0) - 512) / 512.0);
  joyY = 0.8 * joyY + 0.2 * ((analogRead(A1) - 512) / 512.0);
  
  int button = digitalRead(2) == LOW ? 1 : 0;
  
  Serial.print(joyX);
  Serial.print(",");
  Serial.print(joyY);
  Serial.print(",");
  Serial.println(button);
  
  delay(50);
}
