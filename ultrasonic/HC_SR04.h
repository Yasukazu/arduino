#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>

//#define CM true
//#define INCH false
enum RangeUnit {CM, INCH};
#define CM2MKS(CM) CM*58

class HC_SR04 {
    static void _echo_isr();
    static HC_SR04* _instance;
    static HC_SR04* instance(){ return _instance; }
    static int _int;
  protected:
    int _trigger, _echo;  
    volatile unsigned long _start, _end;
    volatile bool _finished;
  public:
    HC_SR04(int trigger, int echo);//, int interrupt=0, int max_dist=200);
    virtual void begin();
    void start();
    bool isFinished(){ return _finished; }
    unsigned int getRange(RangeUnit unit=CM) {   return (_end - _start) / ((unit == CM) ? 58 : 148); }
    unsigned int getRangeRaw(){return (_end - _start); }
    static int getInt() { return _int;};
};

#endif
