#ifndef LETS_COMMUNICATE_H
#define LETS_COMMUNICATE_H

/* LETS COMMUNICATE ASSUMES ARDUINO IDE >= 100 */
#include <Arduino.h>

/* ARDUINO INTERRUPT LIBRARY & DEFINED STRUCTURES */
/* https://github.com/GreyGnome/EnableInterrupt/wiki */
#define LIBCALL_ENABLEINTERRUPT
#define EI_ARDUINO_INTERRUPTED_PIN
#include "EnableInterrupt.h"
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
#define DIGSIZE 13 //13 Digital Pins
#define DXTSIZE 6
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

class LetsCommunicate: public Communicate {
public:
  /* Default CTOR - DOES NOT NOTHING - CONSTRUCTOR 0*/
  LetsCommunicate() { Serial.println("Hello From LETSCommunicate"); };

    /* CUSTOM CONSTRUCTOR 1
    (All Pins (11 pins) will be declare as INPUT)
    PARAMETERS:
    comm_type = {HARDSERIAL, SOFTSERIAL, ISQUAREDC}, source = {CLIENT},
    target = {target}, action = {DIG, DXT, ANA, ALL} */
  LetsCommunicate(uint8_t comm_type, uint8_t source, uint8_t target):
    Communicate(comm_type) {
      // Setup the initial State for LetCommunicate3 Object....
      (*this).comm_type = comm_type;
      (*this).source = source;
      (*this).target = target;
      //(*this).action = action;
      //initConfiguration(1);
    };

    /* DIG OR DXT
    CUSTOM CONSTRUCTOR 2
    PARAMETERS:
    comm_type = {HARDSERIAL, SOFTSERIAL, ISQUAREDC}, source = {CLIENT},
    target = {target}, action = {DIG, DXT, ANA, ALL}, configIO = {0 (OUTPUT),1 (INPUT)} (Index as the pins number),
    size = {How many pins are used}
    */
    LetsCommunicate(uint8_t comm_type, uint8_t source, uint8_t target, uint8_t action, uint8_t * configIO, uint8_t size):
      Communicate(comm_type) {
        (*this).comm_type = comm_type;
        (*this).source = source;
        (*this).target = target;
        (*this).action = action;
        (*this).configIO = configIO;
        (*this).size = size;
        //Serial.println("dig with IO");
        //initConfiguration(2);
      };

  /* Initialise With Methods Describe the Intentions
     initialiseWith(uint8_t); Initialise {DIG, DXT, ANA, ALL}
        DIG = ALL DIG IO AS INPUT (INTERRUPT ENABLED BY DEFAULT)
        DXT = ALL DIG IO & ANALOG AS DIGITAL INPUT (INTERRUPT ENABLED BY DEFAULT)
        ANA = ALL ANALOG IO AS INPUT (NO INTERRUPTS)
  */
  void initialiseInputAs(uint8_t);
  void initialiseInputWith(uint8_t, uint8_t);
  void status();
  //void initConfiguration(uint8_t);
  void run();
  void pinState();
  void pinActiveProsses(uint8_t);

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
  uint8_t flag;
  uint8_t * digStateArray;
  uint8_t * interruptStateArray;
  //array to store the value for analog input
  uint8_t * interruptStateArrayANA;

  /* INTERRUPT PRIVATE VARIABLES */
  void configureInterrupts(uint8_t);
  /* FINISH INTERRUPT VARIABLES */
};

#endif // LETS_COMMUNICATE_H
