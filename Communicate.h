#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <Arduino.h>

class Communicate {
public:
  Communicate();
  Communicate(uint8_t comm_type, uint8_t source, uint8_t target, uint8_t action = 0);
  void initConfiguration();

private:
  uint8_t comm_type;
  uint8_t source; // MY_ID
  uint8_t target; // THEIR_ID
  uint8_t action; // DIG, DXT, ANA or DIG + ANA
};
#endif // COMMUNICATE_H
