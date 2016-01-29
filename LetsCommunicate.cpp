#include "LetsCommunicate.h"

void LetsCommunicate::status() { (*this).initConfiguration(); }
void LetsCommunicate::initConfiguration() {
Serial.println((*this).comm_type);
Serial.println((*this).source);
Serial.println((*this).target);
Serial.println((*this).action);
}

void LetsCommunicate::run() {
  // Waiting for something to happen...
  Serial.println("It is inside LetsCom run");
}

void LetsCommunicate::pinActive(uint8_t _pinNumber) {
  // Waiting for something to happen...
  Serial.print("Pin");
  Serial.print(_pinNumber);
  Serial.print("is high");
  Serial.println();
}
