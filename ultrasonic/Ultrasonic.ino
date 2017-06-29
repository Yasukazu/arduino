#include "SubHC_SR04.h"

#define ECHO_PIN 2
#define TRIG_PIN 4

#define ECHO2PIN 3
#define TRIG2PIN 7

#define FRONT_LED_PIN 12
#define SIDE_LED_PIN 12
//#define OBLED 13
#define NEAR1 20
#define FAR1 30

#define INTERVAL 5000 //ms

class Outport {
  byte port;
  int level;
  public:
  Outport(byte p) : port(p), level(LOW) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
  };
  void set(int l) {
    switch(l) {
      case LOW:
      if (level != l) {
        level = l;
        digitalWrite(port, l);
      }
      return;
      case HIGH:
      if (level != l) {
        level = l;
        digitalWrite(port, l);
      }
      return;    
    }
  }
  void on() {
    if (level != HIGH) {
      digitalWrite(port, HIGH);
      level = HIGH;
    }
  }
  void off() {
    if (level != LOW) {
      digitalWrite(port, LOW);
      level = LOW;
    }
  }
  int get() { return level;}
};
//Outport ledPort(OBLED);

class RangeOutport {
  Outport _port;
  unsigned int _low;
  unsigned int _high;
  public:
  RangeOutport(byte port, unsigned int low, unsigned int high) : _port(port),_low(low),_high(high) {}
  void update(unsigned int range) {
    if (range < _low || range > _high) {
      _port.off();
    }
    else if (range >= _low && range <= _high) {
      _port.on();
    }
  }
};

class RengeHC_SR04 : public HC_SR04 {
  RangeOutport _port;
  static int _int;
  static RengeHC_SR04* _instance;
  static RengeHC_SR04* instance(){ return _instance; }
  static void _echo_isr(){
    RengeHC_SR04 *_this = instance();  
    switch(digitalRead(_this->_echo)) {
      case HIGH:
        _this->_start = micros();
        break;
      case LOW:
        _this->_end = micros();
        _this->_port.update(_this->getRangeRaw());
        /*
        if (_this->getRangeRaw() < CM2MKS(NEAR1)){
          ledPort.set(HIGH);
        }
        else if (_this->getRangeRaw() >= CM2MKS(NEAR2)){
          ledPort.set(LOW);
        } */
        _this->_finished = true;
        break;
    }   
  }
  public:
  RengeHC_SR04(int trigger, int echo,  byte port, unsigned int low, unsigned int high) : _port(port, low, high),HC_SR04(trigger, echo) {}
  virtual void begin(){
    pinMode(_trigger, OUTPUT);
    digitalWrite(_trigger, LOW);
    pinMode(_echo, INPUT);  
    _int = digitalPinToInterrupt(_echo);
    attachInterrupt(_int, _echo_isr, CHANGE);
    _instance = this;    
  }
};
RengeHC_SR04 *RengeHC_SR04::_instance(NULL);
int RengeHC_SR04::_int(0);
RengeHC_SR04 sensor(TRIG_PIN, ECHO_PIN, FRONT_LED_PIN, CM2MKS(NEAR1), CM2MKS(FAR1));
//HC_SR04 sensor(TRIG_PIN, ECHO_PIN);
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
  // ledPort.set(HIGH);  delay(1000);  ledPort.set(LOW);  
}
unsigned long lastMs = millis();
const unsigned int interval = INTERVAL;
void loop(){
  unsigned long ms = millis();
  if (ms > lastMs + interval) {
    for (int i = 0; i < 1; ++i) {
      if (sensors[i]->isFinished()) {
        Serial.print(i == 0 ? "" : "                                ");
        Serial.print("Sensor ");
        Serial.print(i + 1);
        Serial.print("(interrupt:");
        Serial.print(i == 0 ? sensor.getInt() : sensor2.getInt());
        Serial.print(") is: ");
        Serial.print(sensors[i]->getRangeRaw() / 58);
        Serial.println(" cm."); 
        /*
        if (i == 0) {
          if (sensors[i]->getRangeRaw() < CM2MKS(NEAR1)){
            ledPort.set(HIGH);
          }
          else {
            ledPort.set(LOW);
          }
        } */
        sensors[i]->start();
      }
    }
    lastMs = ms;
  }
}