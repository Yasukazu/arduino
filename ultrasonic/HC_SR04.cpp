#include "HC_SR04.h"

int HC_SR04::_int(0);
HC_SR04 *HC_SR04::_instance(NULL);

HC_SR04::HC_SR04(int trigger, int echo)
    : _trigger(trigger), _echo(echo), _finished(false)
{
}

void HC_SR04::begin(){
  pinMode(_trigger, OUTPUT);
  digitalWrite(_trigger, LOW);
  pinMode(_echo, INPUT);  
  _int = digitalPinToInterrupt(_echo);
  attachInterrupt(_int, _echo_isr, CHANGE); // attachInterrupt(_int,
  _instance = this;    
}

void HC_SR04::start(){
  _finished=false;
  digitalWrite(_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigger, LOW);  
}

void HC_SR04::_echo_isr(){
  HC_SR04 *_this = instance();  
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
