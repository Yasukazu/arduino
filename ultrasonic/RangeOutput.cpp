#include <Arduino.h>
#include "RangeOutput.h"

RangeOutput::RangeOutput(int low, int high, int input)
: _low(low),_high(high),_input(input),_range(UNDEF) {
}

int RangeOutput::update(unsigned long input) {
  int cond = (_low >= 0 ? 1 : 0) | (_high >= 0 ? 2 : 0);
  switch(cond) {
    case 0:
      break;
    case 1: // ignore _high
      if (input < _low) {
        _range = UNDER;
      }
      else {
        _input = input;
        _range = IN;
      }
      break;
    case 2: // ignore _low
      if (input > _high) {
        _range = OVER;
      }
      else {
        _input = input;
        _range = IN;
      }
    default:
      if (input < _low || input > _high) {
        _range = input < _low ? UNDER : OVER;
      }
      else if (input >= _low && input <= _high) {
        _input = input;
        _range = IN;
      }
  }
  return _range;
}
