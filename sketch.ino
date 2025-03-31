/*
СКЕТЧ С ОБЩИМ ПЛЮСОМ!
ЕСЛИ 7-МИ СЕГМЕНТНЫЙ С ОБЩИМ МИНУСОМ,
ТО НУЖНО ПОМЕНЯТЬ С + НА - У 7-МИ СЕГМЕНТНОГО
*/
#include <SPI.h>

#define latchPin 10
#define clockPin 12
#define dataPin 11
#define piezoPin 3

const int melody[] = {262, 330, 392, 440, 587, 659, 784};

const byte segments[] = { // сегменты для общего плюса
    0b11000000, 0b11111001, // 0 / 1
    0b10100100, 0b10110000, // 2 / 3
    0b10011001, 0b10010010, // 4 / 5
    0b10000010, 0b11111000, // 6 / 7
    0b10000000, 0b10010000, // 8 / 9
    0b10001100, 0b10010010   // P / S
  };
  
/*
const byte segments[] = { // сегменты для общего минуса
    0b00111111, 0b00000110, // 0 / 1
    0b01011011, 0b01001111, // 2 / 3
    0b01100110, 0b01101101, // 4 / 5
    0b01111101, 0b00000111, // 6 / 7
    0b01111111, 0b01101111, // 8 / 9
    0b01110011, 0b01101101 // P / S
  };
*/

void send74HC595(byte data) {
  // Зажигаем дисплей
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(piezoPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Введите число от 1 до 9:");
  while (!Serial.available()) {}
  int inputNumber = Serial.parseInt();
  
  // Отправляем каждую цифру в 74HC595
  if (inputNumber >= 1 && inputNumber <= 9) {
    for (int num = inputNumber; num >= 0; num--) {
      send74HC595(num >= 0 && num <= 9 ? segments[num] : 0b00000000);
      delay(1000);
    }
    playMelody();
    send74HC595(segments[11]); // Отображаем Stop
  } else if (Serial.available()) Serial.println("Некорректный ввод!");
}

void playMelody() {
  for (int note : melody) {
    tone(piezoPin, note);
    send74HC595(segments[10]); // Отображаем Play
    delay(1000);
    noTone(piezoPin);
    delay(50);
  }
}
