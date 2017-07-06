#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>
#include "Update.h"

//#define CM true
//#define INCH false
enum RangeUnit {CM, INCH};
#define CM2MKS(CM) CM*58
#define MAX_REP_RATE 60 // ms

class HC_SR04 : public Update {
    static HC_SR04* _instance;
    static HC_SR04* instance(){ return _instance; }
    static int _int;
    static void _echo_isr();
  protected:
    int _trigger, _echo;
    volatile unsigned long _start, _end;
    volatile bool _finished;
    unsigned int _interval; // ms
    unsigned long _startMls; // ms
  public:
    HC_SR04(int trigger, int echo, unsigned int interval=MAX_REP_RATE);//, int interrupt=0, int max_dist=200);
    virtual int begin();
    void start();
    bool isFinished(){ return _finished; }
    unsigned int getRange(RangeUnit unit=CM) {   return (_end - _start) / ((unit == CM) ? 58 : 148); }
    unsigned int getRangeRaw(){return (_end - _start); }
    static int getInt() { return _int;}
    int getInterval() { return _interval;}
    void setInterval(int w) { if(w > MAX_REP_RATE) _interval = w; }
    bool isRead;
    virtual int update(unsigned long ms=millis());
};

#endif
