#ifndef SUBHC_SR04_H
#define SUBHC_SR04_H

#include <Arduino.h>
#include "HC_SR04.h"

class SubHC_SR04 : public HC_SR04 {
    static void _echo_isr();
    static SubHC_SR04* _instance;
    static SubHC_SR04* instance(){ return _instance; }
    static int _int;
  public:
    SubHC_SR04(int trigger, int echo);
    virtual void begin();
    static int getInt() { return _int;};
};

#endif
