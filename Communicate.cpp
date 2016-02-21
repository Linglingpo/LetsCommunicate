#include "Communicate.h"

Communicate::Communicate() { }

Communicate::Communicate(uint8_t _mastercomm, uint8_t _this_id) {
  //need "new" when it is pointer
  transmitState = new transmission();
  (*this).transmitState->mastercomm = _mastercomm;
  (*this).transmitState->source = _this_id;
}

Communicate::Communicate(uint8_t _mastercomm, uint8_t _this_id, uint8_t _intercomm) {
  new Communicate(_mastercomm, _this_id);
  (*this).transmitState->intercomm = _intercomm;
}

void serialEvent() {

}

uint8_t Communicate::discover(uint8_t _comm) {

  uint8_t _return = 0;

  switch(_comm) {
    case HARDSERIAL:
        if( (*this).constructPreamble(_comm, (*this).transmitState->source, SYN, (*this).transmitState->master) ) {
          ///send
          send(_comm, (*this).transmitState->master);
        }
      //return 0;
    break;
    case SOFTSERIAL:
      //return 0;
    break;
    case ISQUAREDC:
      //return 0;
    break;
  }
  return _return;
}

uint8_t Communicate::constructPreamble(uint8_t _comm, uint8_t _source, uint8_t _type, transmit & channel) {

  channel.preamble[0] = HELLO;
  channel.preamble[1] = PREAMBLE_SIZE;
  channel.preamble[2] = _source;
  channel.preamble[3] = 0;
  channel.preamble[4] = (!channel.discovered && _type == SYN) ? channel.syn = random(0, 255) : channel.syn;
  channel.preamble[5] = (!channel.discovered && _type == SYN) ? channel.ack = 0 : channel.ack;
  channel.preamble[6] = _type;

  return 1;
}

uint8_t Communicate::send(uint8_t _comm, const transmit & channel) {
  uint8_t _return = 0;


  switch(_comm) {
    case HARDSERIAL:
      //(*this).discover(_comm);
      uint8_t counter = 0;
      uint8_t receive = 0;
      do {
          for(int i = 0; i < PREAMBLE_SIZE; i++)
            Serial.write(channel.preamble[i]);

          if(channel.preamble[6] == CNT)
            //for(int i = 0; i < 14; i++)
              //Serial.write(channel.payload[i]);
          Serial.write("\n");

          Serial.print("ATTEMPT NUMBER: "); Serial.print(counter + 1); Serial.println(" OF THREE ATTEMPTS!");
          receive = (*this).receive(_comm, counter++, channel);
          (*this).stopWatchStop();
          (*this).stopWatchReset();
    } while (counter != MAX_ATTEMPTS && receive != 1);

      if(counter >= MAX_ATTEMPTS || receive != 1) { return 0; }


      break;
  }


  return _return;
}


uint8_t Communicate::receive(uint8_t _comm, uint8_t _counter, const transmit & channel) {
  uint8_t _return = 0;

  switch(_comm) {
    case HARDSERIAL:
      uint8_t _response;
      uint32_t _permittedTime = 0UL;

      (*this).stopWatchStart();
      do {
          _response = Serial.read();
          _permittedTime = (*this).elapsed();
      } while( _permittedTime < ( DEFAULTPERIOD + (DEFAULTPERIOD * _counter) ) && _response != HELLO);

      if(_response != HELLO || _counter >= MAX_ATTEMPTS) { return 0; }


      break;
  }


  return _return;
}

// uint8_t Communicate::send(uint8_t _comm, const transmit & channel) {
//
//   uint8_t _return = -1;
//   // determine where to send?????
//   switch(_comm) {
//     case 0: // HARDSERIAL
//     // A sequence of events to successfully send or not send....
//
//         (*this).discover(_comm);
//         //Serial.println((*this).transmitState->master.startPacket.preamble);
//         for(int i = 0; i < PREAMBLE_SIZE; i++) {
//           Serial.write(channel.preamble[i]);
//         }
//         Serial.write("\n");
//
//         // Wating for Computer to Repond....
//         uint8_t _response = Serial.read();
//         while(_response != HELLO ) { Serial.println("Wating for Discovery ... "); _response = Serial.read(); delay(250); }
//
//         /* IF WE ARE HERE - THEN PROCESSING RESPONDED ;) - NOW TO CHECK THE REPONSE */
//         _response = Serial.read(); // PREAMBLE SIZE
//         uint8_t * received = new uint8_t[_response - 2];
//
//         for(int i = 0; i < _response - 2; i++) {
//           received[i] = Serial.read();
//         }
//
//         /* IS ARDUINO ORIGINAL SYN == RECEIVED ACK */
//         if(received[3] == (*this).transmitState->master.syn) {
//           (*this).transmitState->master.target = received[0];
//           (*this).transmitState->master.syn = ++received[2];
//         }
//
//         /* SEND FINISH MESSAGE - WE HAVE DISCOVERED AND SYNCHED */
//         (*this).transmitState->master.preamble[0] = HELLO;
//         (*this).transmitState->master.preamble[1] = 7; // 5
//         (*this).transmitState->master.preamble[2] = (*this).transmitState->source;
//         (*this).transmitState->master.preamble[3] = (*this).transmitState->master.target;
//         (*this).transmitState->master.preamble[4] = (*this).transmitState->master.syn;
//         (*this).transmitState->master.preamble[5] = (*this).transmitState->master.ack;
//         (*this).transmitState->master.preamble[6] = FIN;
//
//         //Serial.println((*this).transmitState->master.startPacket.preamble);
//         for(int i = 0; i < 7; i++) {
//           Serial.write((*this).transmitState->master.preamble[i]);
//         }
//         Serial.write("\n");
//
//
//         _response = Serial.read();
//         while(_response != HELLO ) { Serial.println("Wating for FINISH ... "); _response = Serial.read(); delay(250); }
//
//         for(int i = 0; i < _response - 2; i++) {
//           received[i] = Serial.read();
//         }
//
//         /* IS ARDUINO ORIGINAL SYN == PROCESSING ACK */
//         if(received[3] == (*this).transmitState->master.syn) {
//           (*this).transmitState->master.syn = ++received[2];
//         }
//
//         (*this).transmitState->master.discovered = true;
//         return 1;
//
//     break;
//   }
//   return _return;
// }
