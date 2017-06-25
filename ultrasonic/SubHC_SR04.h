#ifndef SUBHC_SR04_H
#define SUBHC_SR04_H

#include <Arduino.h>
#include "HC_SR04.h"

class SubHC_SR04 : public HC_SR04 {
    static int _int;
    static SubHC_SR04* _instance;
    static SubHC_SR04* instance(){ return _instance; }
    static void _echo_isr();
    int _trigger, _echo;  
    volatile unsigned long _start, _end;
    volatile bool _finished;
  public:
    SubHC_SR04(int trigger, int echo);
    virtual void begin();
    virtual void start();
    virtual bool isFinished(){ return _finished; }
    virtual unsigned int getRange(bool units=CM) {   return (_end - _start) / ((units) ? 58 : 148); }
    virtual unsigned int getRangeRaw(){return (_end - _start); }
    static int getInt() { return _int;};
};

#endif
