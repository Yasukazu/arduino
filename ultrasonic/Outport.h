#ifndef ONOFF_ENUM
#define ONOFF_ENUM
enum OnOff { ON, OFF};
#endif

#ifndef OUTPORT_H
#define OUTPORT_H

#include <Arduino.h>

class Outport {
 protected:
  int _port;
  //int _level;
  bool _negative;
  OnOff _onOff;
 public:
  Outport(int p, bool negative=false);
  virtual void set(OnOff l);
  void on() { _onOff = ON; }//   set(_negative ? LOW : HIGH);  }
  void off() { _onOff = OFF; }//set(_negative ? HIGH : LOW);  }
  OnOff get() { return _onOff;}
};

#endif
