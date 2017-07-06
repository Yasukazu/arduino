arduino
====
A collection of Arduino sketches and libraries

# Ultrasonic
Class-static interrupt-trigger functions in each ultrasonic distance sensor classes: HC_SR04 and SubHC_SR04.
## Usage
```c++
#include "SubHC_SR04.h"
#include "Update.h"

#define arraysizeof(aa) sizeof(aa)/sizeof((aa)[0])
#define ECHO1PIN 2
#define TRIG1PIN 4
#define ECHO2PIN 3
#define TRIG2PIN 7
#define SENS_INTVL 100

HC_SR04 frontSensor(TRIG1PIN, ECHO1PIN, SENS_INTVL);
SubHC_SR04 sideSensor(TRIG2PIN, ECHO2PIN, SENS_INTVL);
HC_SR04 *sensors[] = {&frontSensor, &sideSensor};
Update* updates[] = {&frontSensor, &sideSensor};
void setup(){
  frontSensor.start();
  sideSensor.start();
}
void loop(){
  for (int i = 0; i < arraysizeof(updates); ++i)
    updates[i]->update();
  if (frontSensor.isFinished() && !frontSensor.isRead) {
    unsigned int rawRange = frontSensor.getRangeRaw();
    /* front sensor read routine */
    frontSensor.isRead = true;
  } 
  if (sideSensor.isFinished() && !sideSensor.isRead) {
    /* side sensor read routine */ 
    sideSensor.isRead = true;
  }
}
```
[Click here to jump to Original Web page:](http://www.instructables.com/id/Non-blocking-Ultrasonic-Sensor-for-Arduino/)
