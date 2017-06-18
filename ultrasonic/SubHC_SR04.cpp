#include "SubHC_SR04.h"

//HC_SR04 *SubHC_SR04::_instance=NULL;
SubHC_SR04 *SubHC_SR04::_instance(NULL);

SubHC_SR04::SubHC_SR04(int trigger, int echo, int interrupt, int max_dist)
    : HC_SR04(trigger, echo, interrupt, max_dist)
{
  if(_instance==0) _instance=this;
}

void SubHC_SR04::begin(){
  pinMode(_trigger, OUTPUT);
  digitalWrite(_trigger, LOW);
  pinMode(_echo, INPUT);
  attachInterrupt(digitalPinToInterrupt(_echo), _echo_isr, CHANGE); // attachInterrupt(_int,
}

void SubHC_SR04::start(){
  _finished=false;
  digitalWrite(_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigger, LOW);
}

unsigned int SubHC_SR04::getRange(bool units){
  return (_end-_start)/((units)?58:148);
}

unsigned int SubHC_SR04::getRangeRaw(){
  return (_end-_start);
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
