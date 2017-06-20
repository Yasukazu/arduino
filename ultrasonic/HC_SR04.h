
#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>

#define CM true
#define INCH false
#define CM2MKS(CM) CM*58

class HC_SR04 {
  public:
    HC_SR04(int trigger, int echo);//, int interrupt=0, int max_dist=200);

    virtual void begin();
    virtual void start();
    bool isFinished(){ return _finished; }
    unsigned int getRange(bool units=CM);
    unsigned int getRangeRaw();
    static int getInt() { return _int;};

  protected:
    int _trigger, _echo, _max;
    volatile unsigned long _start, _end;
    volatile bool _finished;
    static int _int;
    static HC_SR04* instance(){ return _instance; }
    static void _echo_isr();
    static HC_SR04* _instance;
};

#endif
