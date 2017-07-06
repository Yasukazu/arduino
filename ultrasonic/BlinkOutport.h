#ifndef BLINKOUTPORT_H
#define BLINKOUTPORT_H

#include <Arduino.h>
#include "Outport.h"
#include "Update.h"

class BlinkOutport : public Outport, public Update {
  bool _blink;
  int _interval; // ms
  unsigned long _last;
  public:
  BlinkOutport(int p, int interval=500, bool negative=false);
  virtual int update(unsigned long ms=millis());
  void setInterval(int interval);
};

#endif
