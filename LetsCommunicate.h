#ifndef LETS_COMMUNICATE_H
#define LETS_COMMUNICATE_H

/* LETS COMMUNICATE ASSUMES ARDUINO IDE >= 100 */
#include <Arduino.h>
#include "StopWatch.h"
#include "Communicate.h"

#define HISTORY_SIZE 3
#define PREAMBLE_SIZE 7
#define PAYLOAD_DIGITAL_SIZE 3
#define PAYLOAD_ANALOG_SIZE 12

#define HELLO 126 // ~
/* PREAMBLE TYPES */
#define SYN   0 // SYN = SYNCHRONISE
#define URG   1 // URG = ACT NOW
#define RST   2 // RST = CLEAR?
#define CNT   3 // CNT = MORE THAN ONE MESSAGE - CHECK FOR FIN
#define FIN   4 // FIN = COMMUNICATIONS FINISHED
/* PAYLOAD MESSAGE TYPE */
#define DIG   252 // DIG = DIGITAL
#define DXT   253 // DXT = DIGITAL EXTENDED
#define ANA   254 // ANA = ANALOG
#define ALL   255 // ALL = DIGITAL + ANALOG
/* SYN RESET CONTROL */
#define MAXMSGS 255
/* COMMUNICATIONS TYPE */
#define HARDSERIAL  0
#define SOFTSERIAL  1
#define ISQUAREDC   2

struct preamble {
  uint8_t preamble[PREAMBLE_SIZE];
};

struct payload {
  uint8_t payload_digital[PAYLOAD_DIGITAL_SIZE];
  uint8_t payload_analog[PAYLOAD_ANALOG_SIZE];
};

class LetsCommunicate: public Communicate, public StopWatch {
public:
  /* Default CTOR - DOES NOT NOTHING */
  LetsCommunicate() { Serial.println("Hello From LETSCommunicate"); };
  /* CUSTOM CONSTRUCTOR
    PARAMETERS:
    comm_type = {HARDSERIAL, SOFTSERIAL, ISQUAREDC}, source = {CLIENT},
    target = {target}, action = {DIG, DXT, ANA, ALL} */
  LetsCommunicate(uint8_t comm_type, uint8_t source, uint8_t target, uint8_t action):
    Communicate(comm_type, source, target, action) { };

  void status();

private:
  preamble * preamble_history[HISTORY_SIZE] = {0};
  payload * payload_history[HISTORY_SIZE] = {0};
  uint8_t syn;
  uint8_t ack;
};

#endif // LETS_COMMUNICATE_H
