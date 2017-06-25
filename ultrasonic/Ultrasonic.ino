#include "SubHC_SR04.h"

#define ECHO_PIN 2
#define TRIG_PIN 4

#define ECHO2PIN 3
#define TRIG2PIN 7

HC_SR04 sensor(TRIG_PIN, ECHO_PIN);
SubHC_SR04 sensor2(TRIG2PIN, ECHO2PIN);
HC_SR04 *sensors[] = {&sensor, &sensor2};

void setup(){
  sensors[0]->begin();
  sensors[1]->begin();
  Serial.begin(9600);
  while(!Serial)
    continue;
  sensors[0]->start();
  sensors[1]->start();
}

void loop(){
  for (int i = 0; i < 2; ++i) {
    if (sensors[i]->isFinished()) {
      Serial.print(i == 0 ? "" : "                                ");
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print("(interrupt:");
      Serial.print(i == 0 ? sensor.getInt() : sensor2.getInt());
      Serial.print(") is: ");
      Serial.print(sensors[i]->getRangeRaw() / 58);
      Serial.println(" cm."); 
      sensors[i]->start();
    }
  }
}