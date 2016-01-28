#ifndef LETS_COMMUNICATE_H
#define LETS_COMMUNICATE_H

/* LETS COMMUNICATE ASSUMES ARDUINO IDE >= 100 */
#include <Arduino.h>
#include "StopWatch/StopWatch.h"
#include "Communicate/Communicate.h"

class LetsCommunicate: public Communicate, public StopWatch {
public:
  /* Default CTOR - DOES NOT NOTHING */
  LetsCommunicate() { Serial.println("Hello From LETSCommunicate"); };
  /* CUSTOM CONSTRUCTOR
    PARAMETERS:
    comm_type = {HARDSERIAL, SOFTSERIAL, ISQUAREDC}, source = {CLIENT},
    target = {target}, action = {DIG, DXT, ANA, ALL} */
  LetsCommunicate(uint8_t comm_type, uint8_t source, uint8_t target, uint8_t action = 0):
    Communicate(comm_type, source, target, action) { };

  void status() { (*this).initConfiguration(); (*this).stopWatchStart(); Serial.println((*this).elapsed()); };

private:

};

#endif // LETS_COMMUNICATE_H
