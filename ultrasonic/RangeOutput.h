#ifndef RANGE_ENUM
#define RANGE_ENUM
enum Range:int { UNDEF=-2, UNDER=-1, IN=0, OVER=1, OUT=2};
#endif

#ifndef RANGEOUTPUT_H
#define RANGEOUTPUT_H

#include "Update.h"

class RangeOutput : public Update {
  int _low;
  int _high;
  int _input;
  Range _range;
  public:
  RangeOutput(int low=-1, int high=-1, int _input=-1);
  virtual int update(unsigned long range); // return as int cast from Range
  Range getRange() { return _range;}
  int getInput() { return _input;}
};

#endif
