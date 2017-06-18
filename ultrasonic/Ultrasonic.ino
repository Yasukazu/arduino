#include "HC_SR04.h"
#include "SubHC_SR04.h"

#define ECHO_PIN 2
#define TRIG_PIN 4

#define ECHO2PIN 3
#define TRIG2PIN 7

/*
class V2HC_SR04 : public HC_SR04 {
  public:
    V2HC_SR04(int trigger, int echo, int interrupt=0, int max_dist=200)
    : HC_SR04(trigger, echo, interrupt, max_dist)
    {
      if(_instance==0) _instance=this;
    }

    virtual bool isFinished(){ 
      Serial.print("Sensor of echo pin ");
      Serial.print(_echo);
      Serial.print(" is: ");
      Serial.print(getRange());
      Serial.println(" 58*cm");    

      return _finished;
    }  
};

class V2SubHC_SR04 : public SubHC_SR04 {
  public:
    V2SubHC_SR04(int trigger, int echo, int interrupt=0, int max_dist=200)
    : SubHC_SR04(trigger, echo, interrupt, max_dist)
    {
      if(_instance==0) _instance=this;
    }

    virtual bool isFinished(){ 
      Serial.print("Sensor of echo pin ");
      Serial.print(_echo);
      Serial.print(" is: ");
      Serial.print(getRange());
      Serial.println(" 58*cm");    

      return _finished;
    }  
};
*/
HC_SR04 sensor(TRIG_PIN, ECHO_PIN, 0, 200, exec);
SubHC_SR04 sensor2(TRIG2PIN, ECHO2PIN, 0, 200, exec);
HC_SR04 *sensors[] = {&sensor, &sensor2};

void setup(){
  sensors[0]->begin();
  sensors[1]->begin();
  Serial.begin(9600);
  while(!Serial)
    continue;
  for (int i = 0; i < 2; ++i)
    sensors[i]->start();
}

void loop(){
//  Serial.print("Start loop:\n");
  for (int i = 0; i < 2; ++i) {
    if (sensors[i]->isFinished()) {
      //unsigned int rawRange = sensors[i]->getRangeRaw();
      sensors[i]->start();
      /*
      Serial.print("Sensor ");
      Serial.print(i+1);
      Serial.print(i == 0 ? ": " : ":          ");
      Serial.print(rawRange / 58);
      Serial.println(" cm");    */
    }
    else {
    }
  }
//  Serial.println(":end loop.");
}

void exec(int echo, unsigned int rawRange) {
      Serial.print("Sensor of echo pin ");
      Serial.print(echo);
      Serial.print(" is: ");
      Serial.print(rawRange / 58);
      Serial.println(" cm.");      
}
/*
unsigned int readSensorRaw(int i) {
  sensors[i]->start();
  while(!sensors[i]->isFinished())
    continue;
  return sensors[i]->getRangeRaw();
}*/

