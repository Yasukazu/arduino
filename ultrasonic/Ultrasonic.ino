#include "HC_SR04.h"
#include "SubHC_SR04.h"
#include "DualMotorDriver.h"
//#include "BtnSw.h"

#define ECHO_PIN 2
#define TRIG_PIN 4

#define ECHO2PIN 3
#define TRIG2PIN 7

#define BTN1_PIN A2
#define BTN2_PIN A3 // R
#define BTN3_PIN A4 // L
#define BTN4_PIN A5

long rawRanges[2];
bool rangeUpdated[2];
//void exec(int echo, unsigned int rawRange);
HC_SR04 sensor(TRIG_PIN, ECHO_PIN);
SubHC_SR04 sensor2(TRIG2PIN, ECHO2PIN);
HC_SR04 *sensors[] = {&sensor, &sensor2};

ButtonSw btn1(BTN1_PIN), btn2(BTN2_PIN), btn3(BTN3_PIN), btn4(BTN4_PIN);
#define BTN_NO 4
ButtonSw* btnsp[] = {&btn1, &btn2, &btn3, &btn4};
bool forward_run;
DualMotorDriver driver(9, 5, 10, 6);

class BtnToDriver {
  bool status[4];
  public:
  void update(){
    for (int i = 0; i < 4; ++i)
      status[i] = btnsp[i]->get();
  }

  bool getStatus(int n){
    update();
    return status[n];
  }

};
BtnToDriver btnToDriver();

void setup(){
  sensors[0]->begin();
  sensors[1]->begin();
  Serial.begin(9600);
  while(!Serial)
    continue;
  for (int i = 0; i < 2; ++i){
    sensors[i]->start();
    rangeUpdated[i] = false;
  }
}

void loop(){
//  Serial.print("Start loop:\n");
  if (btn1.get())
    forward_run = true;
  if (btn4.get())  
    forward_run = false;
  for (int i = 0; i < 2; ++i) {
    if (sensors[i]->isFinished()) { //    if (rangeUpdated[i]) {
      Serial.print(i == 0 ? "" : "                                ");
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print("(interrupt:");
      Serial.print(i == 0 ? sensor.getInt() : sensor2.getInt());
      Serial.print(") is: ");
      Serial.print(sensors[i]->getRangeRaw() / 58);
      Serial.println(" cm."); 
      //rangeUpdated[i] = false;
      sensors[i]->start();
    }
  }
//  Serial.println(":end loop.");
}