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
  uint8_t mastercomm = 0;  // COMMUNICATION TYPE - HARDSERIAL | SOFTSERIAL | ISQUAREDC
  uint8_t intercomm  = 0;  // COMMUNICATION TYPE - SOFTSERIAL | ISQUAREDC
  uint8_t source     = 0;  // MY_ID

  bool interruptsEnabled = false;        // IF FALSE - NO INTERRUPTS ON DIG OR DXT
  bool customConfigOfInputOut = false;   // IF FALSE - THEN USING DIG/DXT or ANA...
  bool payloadType[3] = { 0 }; //[0] = DIG [1] = DIG [2] = ANA

  uint8_t digitalPinCount = 0; // How many digital pins is used (need to read the state)
  uint8_t analogPinCount  = 0; // How many analog pins is used (need to read the state)
  uint8_t analogDataSize  = 0; // Each Analog pin need to have 2 bytes to store its value

  // about to transmit (ready to send)
  uint32_t digitalState;
  uint32_t previousDigitalState;
  
  //Ana
  uint8_t analogState [PAYLOAD_ANALOG_SIZE]; //12 bit
  // just receive (What is reading now)
  //uint8_t * presentDigitalState;
  uint8_t * presentAnalogState;
  uint8_t * currentAnalogState;
  //FLAG - If the state is changed, then transmit the msg
  bool stateChanged = false;
};

class LetsCommunicate: public Communicate {
public:

  /* Default CTOR - SETS STATE STUCT - CONSTRUCTOR 0*/
  /* http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs */
  LetsCommunicate() { };

    /* CUSTOM CONSTRUCTORS 1 - COMPUTER + ARDUINO / ARDUINO + ARDUINO
    (All Pins (12 pins) will be declare as INPUT)
    PARAMETERS:
    _mastercomm = HARDSERIAL (RX/TX), SOFTSERIAL(DEVICES - Any other than RX/TX), ISQUAREDC (I2C - SDA/SCL)
    _this_id = source = {CLIENT}
    _computer_id = target */
    LetsCommunicate(uint8_t _mastercomm, uint8_t _this_id):
    Communicate(_mastercomm, _this_id) {
      state = new letscommunicate();
      (*this).state->mastercomm = _mastercomm;
      (*this).state->source = _this_id;
    };

    /* CUSTOM CONSTRUCTORS 2 - COMPUTER + ARDUINO + ARDUINO
    (All Pins (12 pins) will be declare as INPUT)
    PARAMETERS:
    _mastercomm = HARDSERIAL, SOFTSERIAL, ISQUAREDC
    _this_id = source = {CLIENT}
    _computer_id = target
    _intercomm = HARDSERIAL, SOFTSERIAL, ISQUAREDC */
    LetsCommunicate(uint8_t _mastercomm, uint8_t _this_id, uint8_t _intercomm):
      Communicate(_mastercomm, _this_id, _intercomm) {
        new LetsCommunicate(_mastercomm, _this_id);
        (*this).state->intercomm = _intercomm;
      };

  /* SERIAL PRINTLNS THE CURRENT STATE OF THE LETCOMMUNICATE */
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

  /* FUNTION TO CHECK IF THE PRESENT STATE IS DIFF THAN THE CURRENT STATE */
  //void checkState(uint8_t*, uint8_t*);
  void checkDigitalState(uint32_t, uint32_t);

  /* FUNTION TO CONSTRUCT PREABLE + DATA MSG AND TRANSMIT (SEND) */
  //SOME WORK NEEDS TO DO - OVERRIDE TO COMMUNICATE BY LETSCOMMUNICATE...
  void transmit(uint8_t, uint8_t);//COMMUNICATION TYPE / PAYLOAD TYPE

private:
  /* MAINTAINS CURRENT STATE OF LETS COMMUNICATE */
  letscommunicate * state;
  /* INITIALSE AND ALLOCATE METHODS */
  void initializeInputTypeAndReadPinsState(uint8_t, bool); //PAYLOAD TYPE / INTERRUPT
  void initialiseDIGDXT(uint8_t); //PAYLOAD TYPE
};

#endif // LETS_COMMUNICATE_H
