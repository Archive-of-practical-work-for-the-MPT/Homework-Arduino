#define LEDS 2

#define OFF_ON_BT 13
#define CHANGE_LIGHT 8
#define PRINT_SNP 3

int lightSwitchBt = LOW;
int ledState = 0;
int brightness = 0;

bool blink = false;
char letter;

void setup() {
  pinMode(LEDS, OUTPUT);

  pinMode(OFF_ON_BT, INPUT);
  pinMode(CHANGE_LIGHT, INPUT);
  pinMode(PRINT_SNP, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // 1. вкл / выкл диоды
  boolean btIsUp = digitalRead(OFF_ON_BT);
  
  if (!btIsUp && lightSwitchBt) {
    blink = false;

    delay(50);
    btIsUp = digitalRead(OFF_ON_BT);

    if (!btIsUp) {
      ledState = ledState == 255 ? 0 : 255;
      analogWrite(LEDS, ledState);
    }
  }

  lightSwitchBt = btIsUp;

  // 2. изменение яркости
  if (digitalRead(CHANGE_LIGHT)) {
    blink = false;
    brightness += 51;
    if (brightness > 255) brightness = 0;
    analogWrite(LEDS, brightness); // до 128 не светит в эмуляторе
    delay(200);
  }

  // 3. вывод фио
  if (digitalRead(PRINT_SNP)) {
    blink = false;
    Serial.println("Себежко Александр Андреевич");
    delay(300);
  }

  // проверяем на ввод
  if (Serial.available()) {
    char letter = Serial.read();

    // 4. мигание диодов
    if (letter == 'M') { blink = true; }
    else if (Serial.available() && letter != 'M' && letter != 'S') {
      blink = false;
    }

    // 5. выключение диодов
    else if (letter == 'S') {
      analogWrite(LEDS, 0);
      blink = false;
    }
  }

  // мигание
  if (blink) {
    analogWrite(LEDS, 255);
    delay(200);
    analogWrite(LEDS, 0);
    delay(200);
  }
}