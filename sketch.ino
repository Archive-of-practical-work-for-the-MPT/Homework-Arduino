#define PIN_R 13
#define PIN_Y 8
#define PIN_G 2

void setup() {
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_Y, OUTPUT);
}

void loop() {
  // Включаем и выключаем красный
  digitalWrite(PIN_R, HIGH);
  delay(1000);
  digitalWrite(PIN_R, LOW);

  // Включаем и выключаем желтый
  digitalWrite(PIN_Y, HIGH);
  delay(1000);
  digitalWrite(PIN_Y, LOW);

  // Включаем и выключаем зеленый
  digitalWrite(PIN_G, HIGH);
  delay(1000);
  digitalWrite(PIN_G, LOW);

// Делаем моргающий зеленый
for (int i = 0; i < 4; i++) {
    digitalWrite(PIN_G, HIGH);
    delay(200);
    digitalWrite(PIN_G, LOW);
    delay(200);
}

// Включаем и выключаем желтый
  digitalWrite(PIN_Y, HIGH);
  delay(1000);
  digitalWrite(PIN_Y, LOW);
}