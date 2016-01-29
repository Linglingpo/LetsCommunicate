#ifndef LETS_COMMUNICATE_H
#define LETS_COMMUNICATE_H

/* LETS COMMUNICATE ASSUMES ARDUINO IDE >= 100 */
#include <Arduino.h>
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
#define PWM   251 // PWM = PWM OUTPUT
#define DIG   252 // DIG = DIGITAL
#define DXT   253 // DXT = DIGITAL EXTENDED
#define ANA   254 // ANA = ANALOG
#define ALL   255 // ALL = DIGITAL + ANALOG

#define OFFSET 2 // DIG OFFSET
#define DIGSIZE 13
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

class LetsCommunicate: public Communicate{
public:
  /* Default CTOR - DOES NOT NOTHING */
  LetsCommunicate() {Serial.println("Hello From LETSCommunicate"); };
  /* CUSTOM CONSTRUCTOR
    PARAMETERS:
    comm_type = {HARDSERIAL, SOFTSERIAL, ISQUAREDC}, source = {CLIENT},
    target = {target}, action = {DIG, DXT, ANA, ALL} */
  LetsCommunicate(uint8_t comm_type, uint8_t source, uint8_t target, uint8_t action = 0):
    Communicate(comm_type) {
      // Setup the initial State for LetCommunicate3 Object....
      (*this).comm_type = comm_type;
      (*this).source = source;
      (*this).target = target;
      (*this).action = action;
      Serial.println("dig without IO");
      initConfiguration();
    };

    // DIG OR DXT
    LetsCommunicate(uint8_t comm_type, uint8_t source, uint8_t target, uint8_t action, uint8_t * configIO, uint8_t size):
      Communicate(comm_type) {
        (*this).comm_type = comm_type;
        (*this).source = source;
        (*this).target = target;
        (*this).action = action;
        (*this).configIO = configIO;
        (*this).size = size;
        Serial.println("dig with IO");
        initConfiguration();
      };

  void status();
  void initConfiguration();
  void run();
  void pinActive(uint8_t);

private:
  preamble * preamble_history[HISTORY_SIZE] = {0};
  payload * payload_history[HISTORY_SIZE] = {0};
  uint8_t comm_type;
  uint8_t source; // MY_ID
  uint8_t target; // THEIR_ID
  uint8_t action; // DIG, DXT, ANA or DIG + ANA
  uint8_t syn;
  uint8_t ack;
  uint8_t * configIO;
  uint8_t size;
};

#endif // LETS_COMMUNICATE_H
