#include "Communicate.h"

Communicate::Communicate() { Serial.println("Hello From Communicate"); };
Communicate::Communicate(uint8_t comm_type, uint8_t source, uint8_t target, uint8_t action) {
  (*this).comm_type = comm_type;
  (*this).source = source;
  (*this).target = target;
  (*this).action = action;
}

void Communicate::initConfiguration() {
  Serial.print("Configured comm_type: ");
  Serial.println((*this).comm_type);
  Serial.print("Configured source: ");
  Serial.println((*this).source);
  Serial.print("Configured target: ");
  Serial.println((*this).target);
  Serial.print("Configured action: ");
  Serial.println((*this).action);
}
