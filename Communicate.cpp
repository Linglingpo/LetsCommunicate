#include "Communicate.h"

Communicate::Communicate() { }

Communicate::Communicate(uint8_t _mastercomm, uint8_t _this_id, uint8_t _computer_id, uint8_t _intercomm) {
  //need "new" when it is pointer
  transmitState = new transmission();
  (*this).transmitState->mastercomm = _mastercomm;
  (*this).transmitState->source = _this_id;
  (*this).transmitState->target = _computer_id;
  (*this).transmitState->intercomm = _intercomm;
}

uint8_t Communicate::send(uint8_t _comm) {

  uint8_t _return = -1;
  // determine where to send?????
  switch(_comm) {
    case 0: // HARDSERIAL
    // A sequence of events to successfully send or not send....
      if(!(*this).transmitState->master.discovered) {
        Serial.println("Discovered is FALSE we need to sync...");

        (*this).transmitState->master.syn = random(0, 255);
        (*this).transmitState->master.ack = 0;

        (*this).transmitState->master.startPacket.preamble[0] = HELLO;
        (*this).transmitState->master.startPacket.preamble[1] = 7; // 5
        (*this).transmitState->master.startPacket.preamble[2] = (*this).transmitState->source;
        (*this).transmitState->master.startPacket.preamble[3] = (*this).transmitState->target;
        (*this).transmitState->master.startPacket.preamble[4] = (*this).transmitState->master.syn;
        (*this).transmitState->master.startPacket.preamble[5] = (*this).transmitState->master.ack;
        (*this).transmitState->master.startPacket.preamble[6] = URG;

        //Serial.println((*this).transmitState->master.startPacket.preamble);
        for(int i = 0; i < 7; i++)
        {
          Serial.write((*this).transmitState->master.startPacket.preamble[i]);
        }
        Serial.println("");

        // Wating for Computer to Repond....
        uint8_t _response = Serial.read();
        while(_response != HELLO ) { Serial.println("Wating for Discovery ... "); _response = Serial.read(); delay(250); }
        // #define HELLO 126 // ~
        // /* PREAMBLE TYPES */
        // #define SYN   0 // SYN = SYNCHRONISE
        // #define URG   1 // URG = ACT NOW
        // #define RST   2 // RST = CLEAR?
        // #define CNT   3 // CNT = MORE THAN ONE MESSAGE - CHECK FOR FIN
        // #define FIN   4 // FIN = COMMUNICATIONS FINISHED

        // Initiate the discovery procedure
        //return 0;
      }
    break;
    case 1: // SOFTSERIAL
    // A sequence of events to successfully send or not send.... ONLY ONE DEVICE
    if(!(*this).transmitState->inter.discovered) {
      Serial.println("Discovered is FALSE we need to sync...");
    }
    break;
    case 2: // ISQUAREDC
    // A sequence of events to successfully send or not send....
      if(!(*this).transmitState->inter.discovered) {
        Serial.println("Discovered is FALSE we need to sync...");
      }
    break;
  }
  return _return;
}
