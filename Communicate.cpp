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

        (*this).transmitState->master.syn = random(0, 255);
        (*this).transmitState->master.ack = 0;

        (*this).transmitState->master.startPacket.preamble[0] = HELLO;
        (*this).transmitState->master.startPacket.preamble[1] = 7; // 5
        (*this).transmitState->master.startPacket.preamble[2] = (*this).transmitState->source;
        (*this).transmitState->master.startPacket.preamble[3] = (*this).transmitState->target;
        (*this).transmitState->master.startPacket.preamble[4] = (*this).transmitState->master.syn;
        (*this).transmitState->master.startPacket.preamble[5] = (*this).transmitState->master.ack;
        (*this).transmitState->master.startPacket.preamble[6] = SYN;

        //Serial.println((*this).transmitState->master.startPacket.preamble);
        for(int i = 0; i < 7; i++) {
          Serial.write((*this).transmitState->master.startPacket.preamble[i]);
        }
        Serial.write("\n");

        // Wating for Computer to Repond....
        uint8_t _response = Serial.read();
        while(_response != HELLO ) { Serial.println("Wating for Discovery ... "); _response = Serial.read(); delay(250); }

        /* IF WE ARE HERE - THEN PROCESSING RESPONDED ;) - NOW TO CHECK THE REPONSE */
        _response = Serial.read(); // PREAMBLE SIZE
        uint8_t * received = new uint8_t[_response - 2];

        for(int i = 0; i < _response - 2; i++) {
          received[i] = Serial.read();
        }

        /* IS ARDUINO ORIGINAL SYN == PROCESSING ACK */
        if(received[3] == (*this).transmitState->master.syn) {
          (*this).transmitState->master.syn = ++received[2];
        }

        /* SEND FINISH MESSAGE - WE HAVE DISCOVERED AND SYNCHED */
        (*this).transmitState->master.startPacket.preamble[0] = HELLO;
        (*this).transmitState->master.startPacket.preamble[1] = 7; // 5
        (*this).transmitState->master.startPacket.preamble[2] = (*this).transmitState->source;
        (*this).transmitState->master.startPacket.preamble[3] = (*this).transmitState->target;
        (*this).transmitState->master.startPacket.preamble[4] = (*this).transmitState->master.syn;
        (*this).transmitState->master.startPacket.preamble[5] = (*this).transmitState->master.ack;
        (*this).transmitState->master.startPacket.preamble[6] = FIN;

        //Serial.println((*this).transmitState->master.startPacket.preamble);
        for(int i = 0; i < 7; i++) {
          Serial.write((*this).transmitState->master.startPacket.preamble[i]);
        }
        Serial.write("\n");


        _response = Serial.read();
        while(_response != HELLO ) { Serial.println("Wating for FINISH ... "); _response = Serial.read(); delay(250); }

        for(int i = 0; i < _response - 2; i++) {
          received[i] = Serial.read();
        }

        /* IS ARDUINO ORIGINAL SYN == PROCESSING ACK */
        if(received[3] == (*this).transmitState->master.syn) {
          (*this).transmitState->master.syn = ++received[2];
        }

        (*this).transmitState->master.discovered = true;
        return 1;
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
