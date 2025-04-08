#include <Arduino.h>
#include <SoftwareSerial.h>

#define BT_TX 8
#define BT_RX 9
SoftwareSerial bluetooth(BT_TX, BT_RX);

#define MotLNap 4
#define MotLSp 5
#define MotRNap 7
#define MotRSp 6
#define LEFT_SENSOR 11
#define RIGHT_SENSOR 10

#define BASE_SPEED 150
#define TURN_SPEED 200
#define LINE_SPEED 180
#define BACK_SPEED 130

enum MovementState
{
  STOPPED,
  FORWARD,
  BACKWARD,
  TURNING_LEFT,
  TURNING_RIGHT,
  LINE_FOLLOWING
};

MovementState currentState = STOPPED;
bool lineFollowingMode = false;

void reportStatus()
{
  static unsigned long lastReport = 0;
  if (millis() - lastReport > 2000)
  {
    lastReport = millis();

    String statusMessage = "State: ";
    switch (currentState)
    {
    case STOPPED:
      statusMessage += "STOPPED";
      break;
    case FORWARD:
      statusMessage += "MOVING FORWARD";
      break;
    case BACKWARD:
      statusMessage += "MOVING BACKWARD";
      break;
    case TURNING_LEFT:
      statusMessage += "TURNING LEFT";
      break;
    case TURNING_RIGHT:
      statusMessage += "TURNING RIGHT";
      break;
    case LINE_FOLLOWING:
      statusMessage += "LINE FOLLOWING";
      break;
    }

    statusMessage += " | Sensors: L=";
    statusMessage += digitalRead(LEFT_SENSOR);
    statusMessage += " R=";
    statusMessage += digitalRead(RIGHT_SENSOR);

    Serial.println(statusMessage);
    bluetooth.println(statusMessage);
  }
}

void moveForward(int speed)
{
  currentState = FORWARD;
  digitalWrite(MotLNap, HIGH);
  digitalWrite(MotRNap, HIGH);
  analogWrite(MotLSp, speed);
  analogWrite(MotRSp, speed);
}

void moveBackward(int speed)
{
  currentState = BACKWARD;
  digitalWrite(MotLNap, LOW);
  digitalWrite(MotRNap, LOW);
  analogWrite(MotLSp, speed);
  analogWrite(MotRSp, speed);
}

void turnLeft(int speed)
{
  currentState = TURNING_LEFT;
  digitalWrite(MotLNap, HIGH);
  digitalWrite(MotRNap, HIGH);
  analogWrite(MotLSp, speed * 0.3);
  analogWrite(MotRSp, speed);
}

void turnRight(int speed)
{
  currentState = TURNING_RIGHT;
  digitalWrite(MotLNap, HIGH);
  digitalWrite(MotRNap, HIGH);
  analogWrite(MotLSp, speed);
  analogWrite(MotRSp, speed * 0.3);
}

void stopMotors()
{
  currentState = STOPPED;
  analogWrite(MotLSp, 0);
  analogWrite(MotRSp, 0);
}

void followLine()
{
  int leftSensor = digitalRead(LEFT_SENSOR);
  int rightSensor = digitalRead(RIGHT_SENSOR);

  if (leftSensor == HIGH && rightSensor == HIGH)
  {
    moveForward(LINE_SPEED);
  }
  else if (leftSensor == HIGH)
  {
    analogWrite(MotLSp, LINE_SPEED * 0.4);
    analogWrite(MotRSp, LINE_SPEED);
    currentState = TURNING_LEFT;
  }
  else if (rightSensor == HIGH)
  {
    analogWrite(MotLSp, LINE_SPEED);
    analogWrite(MotRSp, LINE_SPEED * 0.4);
    currentState = TURNING_RIGHT;
  }
  else
  {
    turnLeft(LINE_SPEED * 0.7);
    currentState = TURNING_LEFT;
  }
}

void handleBluetooth()
{
  if (bluetooth.available())
  {
    char command = bluetooth.read();
    Serial.print("Command: ");
    Serial.println(command);

    switch (command)
    {
    case 'A':
      lineFollowingMode = true;
      currentState = LINE_FOLLOWING;
      break;

    case 'M':
      lineFollowingMode = false;
      stopMotors();
      break;

    case 'F':
      lineFollowingMode = false;
      moveForward(BASE_SPEED);
      break;

    case 'B':
      lineFollowingMode = false;
      moveBackward(BACK_SPEED);
      break;

    case 'L':
      lineFollowingMode = false;
      turnLeft(TURN_SPEED);
      break;

    case 'R':
      lineFollowingMode = false;
      turnRight(TURN_SPEED);
      break;

    case 'S':
      lineFollowingMode = false;
      stopMotors();
      break;

    default:
      Serial.println("Unknown command");
    }
  }
}

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);

  pinMode(MotLNap, OUTPUT);
  pinMode(MotLSp, OUTPUT);
  pinMode(MotRNap, OUTPUT);
  pinMode(MotRSp, OUTPUT);

  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  delay(1000);
  for (int i = 0; i < 100; i++)
  {
    digitalRead(LEFT_SENSOR);
    digitalRead(RIGHT_SENSOR);
    delay(10);
  }

  stopMotors();
  Serial.println("System initialized");
}

void loop()
{
  handleBluetooth();

  if (lineFollowingMode)
  {
    followLine();
  }

  reportStatus();
  delay(50);
}