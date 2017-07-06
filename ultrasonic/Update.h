#ifndef UPDATE_H
#define UPDATE_H
#include <Arduino.h>
class Update {
  public:
  virtual int update(unsigned long n=millis());
};
#endif
#ifndef UPDATED_ENUM
#define UPDATED_ENUM
enum Updated : int { NOT_UPDATED=0, UPDATED=1};
#endif