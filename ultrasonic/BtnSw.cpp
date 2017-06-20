#ifndef BtnSw_H
#define BtnSw_H

class ButtonSw {
    byte pin;
    boolean last;
    public:
    ButtonSw(byte p) : pin(p), last(false) {
    }
    bool get() {
        bool cur = digitalRead(pin);
        switch((cur ? 1 : 0) | (last ? 2 : 0)) {
            case 1:
              last = true;
              return true;
            case 2:
              last = false;
              return false;
            default:
              return false;
        }
    }
};

#endif