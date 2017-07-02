#include "SubHC_SR04.h"
#include "Update.h"
#include "BlinkOutport.h"
#include "RangeOutput.h"

#define arraysizeof(aa) sizeof(aa)/sizeof((aa)[0])
#define ECHO1PIN 2
#define TRIG1PIN 4

#define ECHO2PIN 3
#define TRIG2PIN 7
#define BETWEEN_MS 1000
#define FRONT_LED_PIN 12
#define SIDE_LED_PIN 13
#define LED_BLINK_MS 500
//#define OBLED 13
#define FRONT_NEAR 30
#define SIDE_NEAR 20
#define SIDE_FAR 30

#define INTERVAL 1000 //ms
#define NEAR_INTVL 200
#define FAR_INTVL 400
#define valPrint(MSG, VAL) Serial.print(MSG),Serial.println(VAL)
#define objMtdPrint(MSG, OBJ, MTD) Serial.print(MSG), Serial.println(OBJ.MTD())

BlinkOutport sideOutPort(SIDE_LED_PIN);
BlinkOutport frontOutPort(FRONT_LED_PIN, NEAR_INTVL);
RangeOutput frontRangeOutput(CM2MKS(FRONT_NEAR));
RangeOutput sideRangeOutput(CM2MKS(SIDE_NEAR), CM2MKS(SIDE_FAR));
HC_SR04 frontSensor(TRIG1PIN, ECHO1PIN);
SubHC_SR04 sideSensor(TRIG2PIN, ECHO2PIN);
HC_SR04 *sensors[] = {&frontSensor, &sideSensor};

void setup(){
  frontOutPort.on();
  delay(LED_BLINK_MS);
  frontOutPort.off();
  sideOutPort.on();
  delay(LED_BLINK_MS);
  sideOutPort.off();
  delay(BETWEEN_MS);
  Serial.begin(9600);
  while(!Serial)
    continue;
  objMtdPrint("Front 1 Range: ", frontRangeOutput, getRange);
  delay(LED_BLINK_MS);
  int range = frontRangeOutput.update(CM2MKS(FRONT_NEAR + 1));
  valPrint("Front 2 Range: ", range);
  delay(2 * LED_BLINK_MS);
  range = frontRangeOutput.update(FRONT_NEAR - 1);
  valPrint("Front 3 Range: ", range);
  objMtdPrint("Side 1 Range: ", sideRangeOutput, getRange);
  delay(LED_BLINK_MS);
  range = sideRangeOutput.update(CM2MKS(SIDE_NEAR + 1));
  valPrint("Side 2 Range: ", range);
  delay(2 * LED_BLINK_MS);
  range = sideRangeOutput.update(SIDE_FAR + 1);
  valPrint("Side 3 Range: ", range);
  frontSensor.begin();
  objMtdPrint("Front Sensor begin. Intr: ", frontSensor, getInt);
  sideSensor.begin();
  objMtdPrint("Side Sensor begin. Intr: ", sideSensor, getInt);
  frontSensor.start();
  Serial.println("Front Sensor start.");
  sideSensor.start();
  Serial.println("Side Sensor start.");
}

void loop(){
  static unsigned long lastMs = millis();
  const unsigned int interval = INTERVAL;
  unsigned long ms = millis();
  static int lastRange = 0;
  int range;
  frontOutPort.update(ms);
  sideOutPort.update(ms);
  if (ms > lastMs + interval) {
    for (int i = 0; i < arraysizeof(sensors); ++i) {
      if (sensors[i]->isFinished()) {
        Serial.print(i == 0 ? "" : "                                ");
        Serial.print(i == 0 ? "Front " : "Side");
        Serial.print("Sensor ");
        Serial.print("(interrupt:");
        Serial.print(i == 0 ? frontSensor.getInt() : sideSensor.getInt());
        Serial.print(") is: ");
        Serial.print(sensors[i]->getRangeRaw() / 58);
        Serial.print(" cm. ");
      if(i == 0) {
        range = frontRangeOutput.update(frontSensor.getRangeRaw());
        switch (range) {
          case UNDER:
            frontOutPort.on();
            break;
          case IN:
            frontOutPort.off();
        }
      }
      else {
        range = sideRangeOutput.update(sideSensor.getRangeRaw());
        switch (range) {
          case UNDER:
            sideOutPort.setInterval(NEAR_INTVL);
            sideOutPort.on();
            break;
          case IN:
            sideOutPort.off();
            break;
          case OVER:
            sideOutPort.setInterval(FAR_INTVL);
            sideOutPort.on();
        }
      }
        //Serial.print("Updated is:");
        //int updated = (i == 0 ? frontSensor.getUpdated() : sideSensor.getUpdated());
        Serial.println();
        sensors[i]->start();
      }
    }
    lastMs = ms;
  }
}
