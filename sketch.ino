#include <SoftwareSerial.h>

// Настройка Bluetooth
SoftwareSerial BTSerial(2, 3); // RX, TX

// Пины
const int ledPins[] = {5, 6, 7, 8, 9, 10};
const int buttonPin = 11;

// Переменная для хранения текущего режима
String currentMode = "";
void setup() {
  // Инициализация Bluetooth
  BTSerial.begin(9600);
  Serial.begin(9600);
  for (int ledPin : ledPins) { pinMode(ledPin, OUTPUT); }
  pinMode(buttonPin, OUTPUT);
  BTSerial.println("Sebezhko P50 ");
}

void loop() {
  // Обработка команд от Bluetooth
  if (BTSerial.available()) {
    String command = BTSerial.readStringUntil('\n');
    command.trim();

    if (command == "MODE_1") {
      currentMode = "MODE_1";
      BTSerial.println("Mode 1 all leds");
    } else if (command == "MODE_2") {
      currentMode = "MODE_2";
      BTSerial.println("Mode 2: blink even odd");
    } else if (command == "MODE_3") {
      currentMode = "MODE_3";
      BTSerial.println("Mode 3: fade");
    } else if (command == "MODE_CHAOS") {
      currentMode = "MODE_CHAOS";
      BTSerial.println("Mode CHAOS: random brightness");
    }
  }

  // Выполнение текущего режима
  if (currentMode == "MODE_1") {
    allLedsOn();
  } else if (currentMode == "MODE_2") {
    blinkEvenOdd();
  } else if (currentMode == "MODE_3") {
    fade();
  } else if (currentMode == "MODE_CHAOS") {
    randomBrightness();
  }

  // Обработка нажатия кнопки
  if (digitalRead(buttonPin) == HIGH) {
    for (int ledPin : ledPins) { digitalWrite(ledPin, LOW); }
    BTSerial.println("End."); 
    currentMode = "";
    delay(100);
  }
}

// Режим 1: Светодиоды просто горят
void allLedsOn() {
  for (int ledPin : ledPins) { digitalWrite(ledPin, HIGH); }
}

// Режим 2: Светодиоды моргают (четный-нечетный)
void blinkEvenOdd() {
  bool showEven = (millis() / 500) % 2 == 0; 
  for (byte i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], ((i % 2 == 0) == showEven) ? HIGH : LOW);
  }
  delay(200);
}

// Режим 3: Плавное загорание и гашение
void fade() {
  static int brightness = 0;
  static int direction = 5;
  for (int ledPin : ledPins) { analogWrite(ledPin, brightness); }
  brightness += direction;
  if (brightness <= 0 || brightness >= 255) { direction = -direction; }
  delay(30);
}

// Режим CHAOS: Хаотичное горение
void randomBrightness() {
  for (int ledPin : ledPins) { analogWrite(ledPin, random(256)); }
  delay(100);
}