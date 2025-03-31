#include <Wire.h>

#define LED 3

byte brightness;
int count;

void setup() {
  pinMode(LED, OUTPUT);
  Wire.begin(7);
  Wire.onReceive(receiveHandler);
  Wire.onRequest(requestHandler);
  Serial.begin(9600);
}

void loop() {
  if (brightness > 10) {
   count += 1;
    delay(1000);
  } else { count = 0; }
}

void receiveHandler(int bytes) {
  if (Wire.available()) {
    brightness = Wire.read();
    if (brightness <= 10) { brightness = 0; }
    analogWrite(LED, brightness); 
  }
}

void requestHandler() {
 Wire.write(count >= 5 ? 1 : 0);
}