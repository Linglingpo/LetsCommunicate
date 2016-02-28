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

struct letscommunicate {
  uint8_t mastercomm = -1;  // COMMUNICATION TYPE - HARDSERIAL | SOFTSERIAL | ISQUAREDC
  uint8_t intercomm = -1;   // COMMUNICATION TYPE - SOFTSERIAL | ISQUAREDC
  uint8_t source = -1;     // MY_ID

  bool interruptsEnabled = false;         // IF FALSE - NO INTERRUPTS ON DIG OR DXT
  bool customConfigOfInputOut = false;    // IF FALSE - THEN USING DIG/DXT or ANA...
  bool payloadType[3] = {0}; // [0] = DIG [1] = DIG [2] = ANA

  uint8_t digitalPinsAllocatedNonCustom = -1;
  uint8_t analogPinsAllocatedNonCustom = -1;
  //uint32_t interruptState = 0;
  uint8_t * readAnalogRead;
  uint8_t * readDigitalRead;
  uint8_t digitalPinsSize = -1;
  uint8_t analogPinsSize = -1;
};

class LetsCommunicate: public Communicate {
public:

  /* Default CTOR - SETS STATE STUCT - CONSTRUCTOR 0*/
  /* http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs */
  LetsCommunicate() { };

    /* CUSTOM CONSTRUCTOR 1
    (All Pins (11 pins) will be declare as INPUT)
    PARAMETERS:
    _mastercomm = USB | BLUETOOTH | WIFI
    _this_id = source = {CLIENT}
    _computer_id = target
    _intercomm = {HARDSERIAL, SOFTSERIAL, ISQUAREDC} */
  LetsCommunicate(uint8_t _mastercomm, uint8_t _this_id):
    Communicate(_mastercomm, _this_id) {
      state = new letscommunicate();
      (*this).state->mastercomm = _mastercomm;
      (*this).state->source = _this_id;
    };

    LetsCommunicate(uint8_t _mastercomm, uint8_t _this_id, uint8_t _intercomm):
      Communicate(_mastercomm, _this_id, _intercomm) {
        new LetsCommunicate(_mastercomm, _this_id);
        (*this).state->intercomm = _intercomm;
      };

  /* SERIAL PRINTLNs THE CURRENT STATE OF THE LETCOMMUNICATE */
  void stateOfTheUnion();

  /* INITIALSE AND ALLOCATE METHODS */
  void initialiseInputAs(uint8_t); // INIT: DIG, DXT, ANA (ALL PINS NO CUSTOM SETS)
  void initialiseInputWithInterruptsAs(uint8_t); // INIT: DIG, DXT (ALL PINS NO CUSTOM SETS)
  void run();

  // Set & Get
  /* The pointer that points to the constant struct (so that ppl can only read)
    1st const the return type that is not able to change,
    2nd const the function annot change the value in struct */
  const letscommunicate * getLetsCommunicateState() const;

  /* FUNTION TO CONSTRUCT PREABLE + DATA MSG AND TRANSMIT (SEND) */
  void transmit(uint8_t, uint8_t);//COMMUNICATION TYPE / PAYLOAD TYPE

private:
  /* MAINTAINS CURRENT STATE OF LETS COMMUNICATE */
  letscommunicate * state;
  /* INITIALSE AND ALLOCATE METHODS */
  void selectAndInitialiseInputAs(uint8_t, bool);
  void initialiseDIGDXT(uint8_t);
};

#endif // LETS_COMMUNICATE_H
