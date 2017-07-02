#include <Arduino.h>
#include "Outport.h"

Outport::Outport(int p, bool negative) : _port(p), _negative(negative), _onOff(OFF) {
  pinMode(p, OUTPUT);
  digitalWrite(p, _negative ? HIGH : LOW);
};

void Outport::set(OnOff nf) {
  if (_onOff != nf) {
    _onOff = nf;
    switch (_onOff) {
      case ON:
        digitalWrite(_port, _negative ? LOW : HIGH);
      case OFF:
        digitalWrite(_port, _negative ? HIGH : LOW);
    }
  }
}
