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
