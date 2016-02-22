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

      while(_response != HELLO) { }
      
      Serial.print("IN SERIAL EVENT!!!!!!");
      Serial.print(Serial.read(), DEC);
      delay(5);
      Serial.print("\n");
    }
    }
  }
}

uint8_t Communicate::discover(uint8_t _comm) {

  uint8_t _return = 0;

  switch(_comm) {
    case HARDSERIAL:
        (*this).constructPreamble(_comm, (*this).transmitState->source, SYN, (*this).transmitState->master);
        if( send(_comm, (*this).transmitState->master) )
              return 1;

      return 0;
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
  channel.preamble[3] = (!channel.discovered && _type == SYN) ? channel.target = 0 : channel.target;
  channel.preamble[4] = (!channel.discovered && _type == SYN) ? channel.syn = random(0, 255) : channel.syn;
  channel.preamble[5] = (!channel.discovered && _type == SYN) ? channel.ack = 0 : channel.ack;

  switch(_type) {
    case SYN:
      channel.preamble[6] = _type;
    break;

    case FIN:
      channel.preamble[6] = _type;
    break;
  }
  return 1;
}

uint8_t Communicate::send(uint8_t _comm, transmit & channel) {
  uint8_t _return = 0;

  switch(_comm) {
    case HARDSERIAL:
      //(*this).discover(_comm);
      uint8_t counter = 0;
      uint8_t receive = 0;
      do {
          for(int i = 0; i < PREAMBLE_SIZE; i++)
            Serial.write(channel.preamble[i]);

          if(channel.preamble[6] == CNT) { }
            //for(int i = 0; i < 14; i++)
              //Serial.write(channel.payload[i]);
          Serial.write("\n");
          //Serial.print("ATTEMPT NUMBER: "); Serial.print(counter + 1); Serial.println(" OF THREE ATTEMPTS!");
          //receive = (*this).receive(_comm, counter++, channel);
          (*this).stopWatchStop();
          (*this).stopWatchReset();
          counter++;
    } while (counter != MAX_ATTEMPTS && receive != 1);

      /* IF THIS IS TRUE - THEN COMMUICATION FAILED - ERROR - receive has several error codes */
      if(counter >= MAX_ATTEMPTS || receive != 1) { return receive; }

      return 1;
      break;
  }


  return _return;
}


uint8_t Communicate::receive(uint8_t _comm, uint8_t _counter, transmit & channel) {
  uint8_t _return = 0;

  switch(_comm) {
    case HARDSERIAL:
      uint8_t _response;
      uint8_t * _response_;
      uint32_t _permittedTime = 0UL;

      (*this).stopWatchStart();
      Serial.flush();
      do {
          _response = Serial.read();
          if(_response == HELLO) {
            Serial.read();
            _response_ = new uint8_t[PREAMBLE_SIZE - 2];
            for(int i = 0; i < PREAMBLE_SIZE - 2; i++)
            {
              delay(15);
              Serial.print(Serial.read()); Serial.print(" ");
            }
            Serial.println("");

            break;

          }
          _permittedTime = (*this).elapsed();
      } while( _permittedTime < ( DEFAULTPERIOD + (DEFAULTPERIOD * _counter) ) && _response != HELLO);

      /* IF THIS IS TRUE - THEN COMMUICATION FAILED - ERROR */
      if(_response != HELLO || _counter >= MAX_ATTEMPTS) { return 0; }


      /* IF THIS IS TRUE - THEN COMMUICATION SUCCEEDED - LETS PEEK INSIDE */
      //return (*this).peek(_comm, channel);
      break;
  }
  return _return;
}
//
// uint8_t Communicate::peek(uint8_t _comm, transmit & channel) {
//
//
//
//   /* SWITCH TO THE RECEIVED CASE */
//   switch(received[4]) {
//     case SYN:
//       /* IS ARDUINO ORIGINAL SYN == RECEIVED ACK */
//       if(received[3] == channel.syn) {
//         channel.target = received[0];
//         channel.syn = ++received[2];
//         channel.ack = received[3];
//         channel.discovered = true;
//       }
//       /* SEND FINISH MESSAGE - WE HAVE DISCOVERED AND SYNCHED */
//       return 1;
//       break;
//     case URG:
//       break;
//     case RST:
//       break;
//     case CNT:
//       break;
//     case FIN:
//       break;
//   }
//
// }

// uint8_t Communicate::send(uint8_t _comm, const transmit & channel) {
//
//   uint8_t _return = -1;
//   // determine where to send?????
//   switch(_comm) {
//     case 0: // HARDSERIAL

//
//         // Wating for Computer to Repond....
//         uint8_t _response = Serial.read();
//         while(_response != HELLO ) { Serial.println("Wating for Discovery ... "); _response = Serial.read(); delay(250); }
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
