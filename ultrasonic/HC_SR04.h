#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>

#define CM true
#define INCH false
#define CM2MKS(CM) CM*58

class HC_SR04 {
    static int _int;
    static HC_SR04* _instance;
    static HC_SR04* instance(){ return _instance; }
    static void _echo_isr();
    int _trigger, _echo;  
    volatile unsigned long _start, _end;
    volatile bool _finished;
  public:
    HC_SR04(int trigger, int echo);//, int interrupt=0, int max_dist=200);
    virtual void begin();
    virtual void start();
    virtual bool isFinished(){ return _finished; }
    virtual unsigned int getRange(bool units=CM) {   return (_end - _start) / ((units) ? 58 : 148); }
    virtual unsigned int getRangeRaw(){return (_end - _start); }
    static int getInt() { return _int;};
};

#endif
