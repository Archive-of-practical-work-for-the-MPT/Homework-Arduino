#include <Stepper.h>
#include <Servo.h>

#define POTEN A0
#define STEPS 32

Servo servo;
Stepper stepper(STEPS, 8, 9, 10, 11);

const int BASE_SPEED = 400;  // Базовая скорость шагового двигателя
const float SERVO_SCALE = 0.7; // Множитель для угла сервопривода
const float SPEED_SCALE = 0.7; // Множитель для скорости шагового двигателя

void setup() {
  pinMode(POTEN, INPUT);
  Serial.begin(9600);
  servo.attach(3);
  stepper.setSpeed(BASE_SPEED);
}

void loop() {
  int potValue = analogRead(POTEN) / 4;
  Serial.println(potValue);
  servo.write(potValue * SERVO_SCALE); // Устанавливаем угол сервопривода
  stepper.setSpeed(BASE_SPEED + potValue * SPEED_SCALE); // Устанавливаем скорость шагового двигателя
  stepper.step(1);
}