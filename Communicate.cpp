#include "Communicate.h"

Communicate::Communicate() { Serial.println("Hello From Communicate"); }

Communicate::Communicate(uint8_t comm_type) {
  (*this).comm_type = comm_type;
}
