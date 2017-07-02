#include <Arduino.h>
#include "BlinkOutport.h"

BlinkOutport::BlinkOutport(int p, int interval, bool negative)
 : Outport(p, negative),_blink(false),_interval(interval),_last(millis())
{
}

int BlinkOutport::update(unsigned long ms) {
  if (ms > _last + _interval) {
    _blink = !_blink;
    if (_onOff == ON)
      digitalWrite(_port, _negative ? !_blink : _blink);
    else if (_onOff == OFF)
      digitalWrite(_port, _negative ? HIGH : LOW);
    _last = ms;
  }
}

void BlinkOutport::setInterval(int interval) {
  _interval = interval;
}
