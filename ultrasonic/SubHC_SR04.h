#ifndef SUBHC_SR04_H
#define SUBHC_SR04_H

#include <Arduino.h>
#include "HC_SR04.h"

class SubHC_SR04 : public HC_SR04 {
  public:
    SubHC_SR04(int trigger, int echo, int interrupt=0, int max_dist=200, void(*exec_func)(int, unsigned int)=NULL);
    virtual void begin();
    virtual void start();
    virtual bool isFinished(){ return _finished; }
    virtual unsigned int getRange(bool units=CM);
    virtual unsigned int getRangeRaw();

  protected:
    static SubHC_SR04* instance(){ return _instance; }
    static void _echo_isr();
    static SubHC_SR04* _instance;
};

#endif
