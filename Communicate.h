#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <Arduino.h>
#include "StopWatch.h"
//3 why handshke
//reciving and transmeting data

class Communicate: public StopWatch {
public:
  Communicate();
  Communicate(uint8_t);

private:
  uint8_t comm_type;
};
#endif // COMMUNICATE_H
