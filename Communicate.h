#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <Arduino.h>
#include "StopWatch.h"

#define HISTORY_SIZE         0x03
#define PREAMBLE_SIZE        0x07
#define PAYLOAD_DIGITAL_SIZE 0x03 //3 ???
#define PAYLOAD_ANALOG_SIZE  0x0C //12

#define HELLO 0x7E // ~
/* PREAMBLE TYPES */
#define SYN   0x00 // SYN = SYNCHRONISE
#define URG   0x01 // URG = ACT NOW
#define RST   0x02 // RST = CLEAR?
#define CNT   0x03 // CNT = MORE THAN ONE MESSAGE - CHECK FOR FIN
#define FIN   0x04 // FIN = COMMUNICATIONS FINISHED

/* SYN RESET CONTROL */
#define MAXMSGS 0xFF
/* COMMUNICATIONS TYPE */
#define HARDSERIAL  0x00
#define SOFTSERIAL  0x01
#define ISQUAREDC   0x02

/* PAYLOAD MESSAGE TYPE */
#define PWM   0xFB // PWM = PWM OUTPUT
#define DIG   0xFC // DIG = DIGITAL
#define DXT   0xFD // DXT = DIGITAL EXTENDED
#define ANA   0xFE // ANA = ANALOG
#define ALL   0xFF // ALL = DIGITAL + ANALOG

#define OFFSET  0x02 // DIG OFFSET
#define DIGSIZE 0x0E //14 Digital Pins
#define DXTSIZE 0x06 //6 Analog to Digital Pins
#define ANASIZE 0x06 //6 Analog Pins

#define MAX_ATTEMPTS 0x03

//not using yet
struct preamble {
  uint8_t preamble[PREAMBLE_SIZE];
};

//not using yet
struct payload {
  uint8_t payload_digital[PAYLOAD_DIGITAL_SIZE];
  uint8_t payload_analog[PAYLOAD_ANALOG_SIZE];
};

//using this one
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
  // transmissionMsg function that will be call in LetsCommunicate.cpp (communication type, payload type, payload size, *payload stats);
  uint8_t transmissionMsg(uint8_t , uint8_t, uint8_t , uint8_t*);
  uint8_t transmitTotalMsgSize = -1;
  //preamble + data
  uint8_t * masterMsg;



private:
  transmission * transmitState;
  uint8_t constructPreamble(uint8_t, uint8_t, uint8_t, transmit &);
  uint8_t constructData(uint8_t, uint8_t, uint8_t*, transmit &);

  uint8_t constructMaster(uint8_t ,uint8_t , transmit &);

  uint8_t receive(uint8_t, uint8_t, transmit &);
  uint8_t peek(uint8_t, transmit & channel);
  uint8_t reConstructPreamble(uint8_t, uint8_t, uint8_t, transmit &, uint8_t *);
};
#endif // COMMUNICATE_H
