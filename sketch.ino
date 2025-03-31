#include <Wire.h>

#define ZVUK 3 // подключение к ШИМ порту
#define POTEN A2

const int melody[] = {262, 330, 392, 440, 587, 659, 784};

bool sound = false;


void setup() {
  pinMode(ZVUK, OUTPUT);
  pinMode(POTEN, INPUT);
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(7);
  Wire.write(analogRead(POTEN) / 4); // из 10бит в 8бит
  Wire.endTransmission();
  
  Wire.requestFrom(7, 1);
  
  if (Wire.available()) {
     sound = Wire.read();
  }
  
  if (sound) {
    for (int note : melody) {
      tone(ZVUK, note);
      delay(250);
    }
  } else {
      noTone(ZVUK);
  }
}