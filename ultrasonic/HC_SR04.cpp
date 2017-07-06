#include "HC_SR04.h"

int HC_SR04::_int(0);
HC_SR04* HC_SR04::_instance(NULL);

HC_SR04::HC_SR04(int trigger, int echo, unsigned int interval)
    : _trigger(trigger), _echo(echo), _finished(false), _interval(interval)
{
}

int HC_SR04::begin(){
  pinMode(_trigger, OUTPUT);
  digitalWrite(_trigger, LOW);
  pinMode(_echo, INPUT);  
  _int = digitalPinToInterrupt(_echo);
  attachInterrupt(_int, _echo_isr, CHANGE);
  _instance = this;    
  return _int;
}

void HC_SR04::start(){
  if (_trigger == _echo) {
    pinMode(_trigger, OUTPUT);
    digitalWrite(_trigger, LOW);
    delayMicroseconds(10);
  }
  _finished=false;
  digitalWrite(_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigger, LOW);  
  _startMls = millis();
  if (_trigger == _echo) {
    pinMode(_trigger, INPUT);  
  }
  isRead = false;
}

int HC_SR04::update(unsigned long ms) {
  if (_finished && isRead && ms > _startMls + _interval) {
    start();
    return UPDATED;
  }
  else
    return NOT_UPDATED;
}


void HC_SR04::_echo_isr(){
  HC_SR04* _this = instance();  
  switch(digitalRead(_this->_echo)) {
    case HIGH:
      _this->_start = micros();
      break;
    case LOW:
      _this->_end = micros();
      _this->_finished = true;
      break;
  }   
}
