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
#define DIGSIZE 14 //14 Digital Pins
#define DXTSIZE 6 //6 Analog to Digital Pins
#define ANASIZE 6 //6 Analog Pins
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

struct letscommunicate {
  uint8_t mastercomm = 0;  // COMMUNICATION TYPE - HARDSERIAL | SOFTSERIAL | ISQUAREDC
  uint8_t intercomm = 0;   // COMMUNICATION TYPE - SOFTSERIAL | ISQUAREDC
  uint8_t source = 0;     // MY_ID
  uint8_t target = 0;     // THEIR_ID

  bool interruptsEnabled = false;         // IF FALSE - NO INTERRUPTS ON DIG OR DXT
  bool customConfigOfInputOut = false;    // IF FALSE - THEN USING DIG/DXT or ANA...
  bool action[3] = {0}; // [0] = DIG [1] = DIG [2] = ANA

  uint8_t digitalPinsAllocatedNonCustom = 0;
  uint8_t analogPinsAllocatedNonCustom = 0;
  uint32_t interruptState = 0;
  uint8_t * readAnalogRead = {0};
  uint8_t * readDigitalRead = {0};

  /* CUSTOM IO */
  uint8_t * digitalCustomInputPins;
  uint8_t * digitalCustomOutputPins;
  uint8_t * analogCustomInputPins;
  uint8_t * analogCustomOutputPins; // PWM SET

  uint8_t digitalCustomInputPinsAllocated;
  uint8_t digitalCustomOutputPinsAllocated;
  uint8_t analogCustomInputPinsAllocated;
  uint8_t analogCustomOutputPinsAllocated;

};

class LetsCommunicate: public Communicate {
public:
  /* Default CTOR - SETS STATE STUCT - CONSTRUCTOR 0*/
  /* http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs */
  LetsCommunicate() { };

    /* CUSTOM CONSTRUCTOR 1
    (All Pins (11 pins) will be declare as INPUT)
    PARAMETERS:
    comm_type = {HARDSERIAL, SOFTSERIAL, ISQUAREDC}, source = {CLIENT},
    target = {target}, action = {DIG, DXT, ANA, ALL} */
  LetsCommunicate(uint8_t _mastercomm, uint8_t _this_id, uint8_t _computer_id, uint8_t _intercomm):
    Communicate(_mastercomm) {
      state = new letscommunicate();
      (*this).state->mastercomm = _mastercomm;
      (*this).state->source = _this_id;
      (*this).state->target = _computer_id;
      (*this).state->intercomm = _intercomm;
    };

  /* Initialise With Methods Describe the Intentions
     initialiseWith(uint8_t); Initialise {DIG, DXT, ANA, ALL}
        DIG = ALL DIG IO AS INPUT (INTERRUPT ENABLED BY DEFAULT)
        DXT = ALL DIG IO & ANALOG AS DIGITAL INPUT (INTERRUPT ENABLED BY DEFAULT)
        ANA = ALL ANALOG IO AS INPUT (NO INTERRUPTS)
  */

  /* SERIAL PRINTLNs THE CURRENT STATE OF THE LETCOMMUNICATE */
  void stateOfTheUnion();

  /* INITIALSE AND ALLOCATE METHODS */
  void initialiseInputAs(uint8_t); // INIT: DIG, DXT, ANA (ALL PINS NO CUSTOM SETS)
  void initialiseInputWithInterruptsAs(uint8_t); // INIT: DIG, DXT (ALL PINS NO CUSTOM SETS)
  void initialiseInputOutputWithCustomSetsAs(uint8_t, uint8_t *, uint8_t, uint8_t, uint8_t *, uint8_t, uint8_t);
  void initialiseInputOutputWithCustomSetsWithInterruptsAs(uint8_t, uint8_t *, uint8_t, uint8_t, uint8_t *, uint8_t, uint8_t);

  //void initialiseInputAs(uint8_t _action, bool _interruptFlag, uint8_t * _digitalConfigIOSet, uint8_t _digitalConfigIOSize);
  //void initialiseInputWith(uint8_t, bool, uint8_t, bool);
  void status();
  //void initConfiguration(uint8_t);
  void run();
  //void pinState();
  //void pinActiveProsses(uint8_t);
  //void configureDigitalInputOutput(uint8_t, uint8_t *, uint8_t);

private:
  preamble * preamble_history[HISTORY_SIZE] = {0};
  payload * payload_history[HISTORY_SIZE] = {0};

  uint8_t action = 0; // DIG, DXT, ANA or DIG + ANA
  uint8_t syn;
  uint8_t ack;
  //size for all pins
  uint8_t totalPinSize;
  uint8_t * digitalConfigIO;
  uint8_t digitalConfigIOSize;
  uint8_t flag;
  uint8_t * digStateArray;
  uint8_t * interruptStateArray;
  //array to store the value for analog input
  uint8_t * interruptStateArrayANA;
  uint16_t anaReadTempNumber;
  uint8_t * interruptState;
  bool interruptFlag;

  /* INTERRUPT PRIVATE VARIABLES */
  //void configureInterrupts(uint8_t);
  /* FINISH INTERRUPT VARIABLES */

  /* INITIALSE AND ALLOCATE METHODS */
  void selectAndInitialiseInputAs(uint8_t, bool);
  void initialiseDIGDXT(uint8_t);

  /* MAINTAINS CURRENT STATE OF LETS COMMUNICATE */
  letscommunicate * state;
};

#endif // LETS_COMMUNICATE_H
