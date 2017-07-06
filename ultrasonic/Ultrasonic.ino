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
#define SENS_INTVL 100
#define INTERVAL 1000 //ms
#define NEAR_INTVL 200
#define FAR_INTVL 400
#define valPrint(MSG, VAL) Serial.print(MSG),Serial.println(VAL)
#define objMtdPrint(MSG, OBJ, MTD) Serial.print(MSG), Serial.println(OBJ.MTD())

BlinkOutport sideOutPort(SIDE_LED_PIN);
BlinkOutport frontOutPort(FRONT_LED_PIN, NEAR_INTVL);
RangeOutput frontRangeOutput(CM2MKS(FRONT_NEAR));
RangeOutput sideRangeOutput(CM2MKS(SIDE_NEAR), CM2MKS(SIDE_FAR));
HC_SR04 frontSensor(TRIG1PIN, ECHO1PIN, SENS_INTVL);
SubHC_SR04 sideSensor(TRIG2PIN, ECHO2PIN, SENS_INTVL);
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

class Logger {
  public:
  static int logCount;
  static unsigned long loop_count;
  byte _sensors;
  unsigned int frontRange, sideRange;// _rawRanges[2];
  unsigned int _diff;
  Logger():_sensors(0),frontRange(0),sideRange(0),_diff(0){}
  void print() { Serial.print(loop_count),Serial.print(","); Serial.print(_sensors),Serial.print(","); Serial.print(frontRange/58),Serial.print(","); Serial.print(sideRange/58),Serial.print(","); Serial.println(_diff);}
};
unsigned long Logger::loop_count(0);
int Logger::logCount(0);
#define MAXLOG 500
Update* updates[] = {&frontOutPort, &sideOutPort, &frontSensor, &sideSensor};

void loop(){
  int logCount = 0;
  static unsigned long lastMs=0;
  unsigned long ms = micros();
  Logger logger;

  for (int i = 0; i < arraysizeof(updates); ++i)
    updates[i]->update(); // frontOutPort.update(ms);  sideOutPort.update(ms);  frontSensor.update(ms);  sideSensor.update(ms);
  if (frontSensor.isFinished() && !frontSensor.isRead) {
    if (logCount < MAXLOG) {
      logger._sensors |= 1;
      logger.frontRange = frontSensor.getRangeRaw();
    }
    //printSensor(0);
    switch (frontRangeOutput.update(frontSensor.getRangeRaw())) {
      case UNDER:
        frontOutPort.on();
        break;
      case IN:
        frontOutPort.off();
    }
    frontSensor.isRead = true;
  }
  if (sideSensor.isFinished() && !sideSensor.isRead) {
    if (logCount < MAXLOG) {
      logger._sensors |= 2;
      logger.sideRange = sideSensor.getRangeRaw();
    }
    //printSensor(1);
    switch (sideRangeOutput.update(sideSensor.getRangeRaw())) {
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
    sideSensor.isRead = true;
  }
  if (logger.logCount < MAXLOG && logger._sensors > 0) {
    logger._diff = ms - lastMs;
    logger.print();
    logger.logCount++;
  }
  lastMs = ms;
  logger.loop_count++;
}
