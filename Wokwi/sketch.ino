#define LEDS 2

#define S1 13
#define S2 10

int stateS1 = LOW;
int stateS2 = LOW;

#define ZVUK 3 // подключение к ШИМ порту

const int melody[] = {262, 330, 392, 440, 587, 659, 784};

bool blink = false;
bool sound = false;
int speed;


void setup() {
  pinMode(LEDS, OUTPUT);

  pinMode(ZVUK, OUTPUT);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);

  Serial.begin(9600);
  Serial.println("Arduino");
}

void loop() {
  // Первая кнопка
  clickBt(S1, stateS1);

  // Вторая кнопка
  clickBt(S2, stateS2);

  // Обработка C# приложения
  if (Serial.available()) {
    char incom = Serial.read();

    switch (incom) {
      case '1': configureBlink(100); break;
      case '2': configureBlink(500); break;
      case '3': configureBlink(1000); break;
      case 's': sound = true; blink = false; break;
    default: 
      if (Serial.available()) { sound = blink = false; } break;
    }
  }

  // Мигание
  if (blink) {
    digitalWrite(LEDS, !digitalRead(LEDS));
    delay(speed);
    }
  
  // Мелодия
  if (sound) {
    for (int note : melody) {
      tone(ZVUK, note);
      delay(250);
    }
  } else {
      noTone(ZVUK);
  }
}

// Обработчик кнопок
void clickBt(byte bt, byte *state) {
  if (digitalRead(bt) && !*state) {

    delay(50);

    if (!digitalRead(bt)) {
      sound = blink = false;
      Serial.println(bt == S1 ? "Нажата первая кнопка" : "Нажата вторая кнопка");
    }
  }

  *state = digitalRead(bt);
}

// Конфиг для мигания
void configureBlink(int speed_value) {
  sound = false;
  blink = true;
  speed = speed_value;
}
