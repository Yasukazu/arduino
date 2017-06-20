#include "SubHC_SR04.h"

SubHC_SR04 *SubHC_SR04::_instance(NULL);
int SubHC_SR04::_int(0);

SubHC_SR04::SubHC_SR04(int trigger, int echo)
    : HC_SR04(trigger, echo)
{
  //if(_instance==0)    _instance=this;
}

void SubHC_SR04::begin(){
  pinMode(_trigger, OUTPUT);
  digitalWrite(_trigger, LOW);
  pinMode(_echo, INPUT);  
  SubHC_SR04::_int = digitalPinToInterrupt(_echo);
  attachInterrupt(SubHC_SR04::_int, _echo_isr, CHANGE); // attachInterrupt(_int,
  SubHC_SR04::_instance = this;    
}

void SubHC_SR04::_echo_isr(){
  SubHC_SR04* _this=SubHC_SR04::instance();
  switch(digitalRead(_this->_echo)){
    case HIGH:
      _this->_start=micros();
      break;
    case LOW:
      _this->_end=micros();
      _this->_finished=true;
      break;
  }
}
