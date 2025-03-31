#include<EEPROM.h>

#define D1 6
#define D2 5
#define D3 4
#define D4 2

#define S1 13
#define S2 8
#define S3 3

int stateS1 = LOW;
int stateS2 = LOW;

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);

  onLed(); // включаем светодиоды
  
  Serial.begin(9600);
}

void loop() {
  // 1. S1
  switchLed(S1, 0, D1, D2, stateS1);

  // 2. S2
  switchLed(S2, 1, D3, D4, stateS2);

  // 4. S3
  if (digitalRead(S3)) {
    EEPROM.write(0, 255);
    EEPROM.write(1, 255);
    onLed(); // включаем светодиоды
  }
}

void switchLed(byte bt, byte cells, byte D_even, byte D_odd, byte *state) {
  if (digitalRead(bt) && *state) {

    delay(50);

    if (!digitalRead(bt)) {
      // прибавляем одно нажатие в EEPROM
      EEPROM[cells]++;
      analogWrite(D_even, EEPROM.read(cells) % 2 == 0 ? 0 : 255);
      analogWrite(D_odd, EEPROM.read(cells) % 2 == 0 ? 255 : 0);

      // 3. выводим в COM-порт
      Serial.print(bt == S1 ? "S1: " : "S2: ");
      Serial.println(EEPROM.read(cells)); // отсчет с нуля
    }
  }

  *state = digitalRead(bt);
}

void onLed() {
    analogWrite(D1, EEPROM.read(0) % 2 == 0 ? 0 : 255);
    analogWrite(D2, EEPROM.read(0) % 2 == 0 ? 255 : 0);
    analogWrite(D3, EEPROM.read(1) % 2 == 0 ? 0 : 255);
    analogWrite(D4, EEPROM.read(1) % 2 == 0 ? 255 : 0);
}