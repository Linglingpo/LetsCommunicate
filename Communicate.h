#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <Arduino.h>
#include "StopWatch.h"

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

/* SYN RESET CONTROL */
#define MAXMSGS 255
/* COMMUNICATIONS TYPE */
#define HARDSERIAL  0
#define SOFTSERIAL  1
#define ISQUAREDC   2

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

#define MAX_ATTEMPTS 3

struct preamble {
  uint8_t preamble[PREAMBLE_SIZE];
};

struct payload {
  uint8_t payload_digital[PAYLOAD_DIGITAL_SIZE];
  uint8_t payload_analog[PAYLOAD_ANALOG_SIZE];
};

struct transmit {
  bool discovered = false;
  uint8_t preamble[PREAMBLE_SIZE];
  uint8_t payload_digital[PAYLOAD_DIGITAL_SIZE];
  uint8_t payload_analog[PAYLOAD_ANALOG_SIZE];

  uint8_t syn = 0;
  uint8_t ack = 0;
  uint8_t target = 0;     // THEIR_ID
};

struct transmission {
  uint8_t mastercomm = 0;  // COMMUNICATION TYPE - HARDSERIAL | SOFTSERIAL | ISQUAREDC
  uint8_t intercomm = 0;   // COMMUNICATION TYPE - SOFTSERIAL | ISQUAREDC
  uint8_t source = 0;     // MY_ID

  //Arduino to computer (object)
  transmit master = transmit();
  //Arduino to Arduino (object)
  transmit inter = transmit();
  //The number of devices that can connect to arduino
  //tansmit * inter[10] = new transmit();
};


class Communicate: public StopWatch {
public:
  Communicate();
  Communicate(uint8_t , uint8_t);
  Communicate(uint8_t , uint8_t , uint8_t);
  uint8_t send(uint8_t, transmit &);
  uint8_t discover(uint8_t);
  void serialEvent();

private:
  transmission * transmitState;
  uint8_t constructPreamble(uint8_t, uint8_t, uint8_t, transmit &);
  uint8_t receive(uint8_t, uint8_t, transmit &);
  uint8_t peek(uint8_t, transmit & channel);
};
#endif // COMMUNICATE_H
