#include "Communicate.h"
uint8_t * msgFromProcessing;
bool updatePreamble = false;

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

// void serialEvent() {
//
//       while(_response != HELLO) { }
//
//       Serial.print("IN SERIAL EVENT!!!!!!");
//       Serial.print(Serial.read(), DEC);
//       delay(5);
//       Serial.print("\n");
//     }
//     }
//   }
// }

void serialEvent() {
  Serial.available();
  Serial.print("Msg From Processing: ");
  uint8_t temp = Serial.read();
  if(temp == 126){
  updatePreamble = true;
  msgFromProcessing = new uint8_t [PREAMBLE_SIZE];
  // no ~
  for(int i = 0; i < PREAMBLE_SIZE - 1; i++){
  msgFromProcessing[i] = Serial.read();
  Serial.print(msgFromProcessing[i]);
  }
  Serial.println();
  delay(50);
  /*
  Serial.print("updatePreamble: ");
  Serial.print(updatePreamble);
  Serial.println();
  */
  //(*this).reConstructPreamble(SYN, msgFromProcessing);
  //(*this).transmitState->master->preamble[3] = msgFromProcessing[2];
  //(*this).transmitState->master->preamble[5] = msgFromProcessing[4]+1;
//update preamble ??
}else {
  Serial.print("Error");
}
}


uint8_t Communicate::discover(uint8_t _comm) {
  uint8_t _return = 0;
  switch(_comm) {
    case HARDSERIAL:
        (*this).constructPreamble(_comm, (*this).transmitState->source, SYN, (*this).transmitState->master);
        if(send(_comm, (*this).transmitState->master) ) {
          (*this).constructPreamble(_comm, (*this).transmitState->source, FIN, (*this).transmitState->master);
          if(send(_comm, (*this).transmitState->master) )
            return 1;
        }
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

uint8_t Communicate::reConstructPreamble(uint8_t _comm, uint8_t _source, uint8_t _type, transmit & channel, uint8_t * _msgFromProcessing){
  channel.preamble[0] = HELLO;
  channel.preamble[1] = PREAMBLE_SIZE;
  channel.preamble[2] = _source;
  channel.preamble[3] = (!channel.discovered && _type == SYN) ? channel.target = _msgFromProcessing[2] : channel.target;
  channel.preamble[4] = (!channel.discovered && _type == SYN) ? channel.syn = random(0, 255) : channel.syn;
  channel.preamble[5] = (!channel.discovered && _type == SYN) ? channel.ack = _msgFromProcessing[4]+1 : channel.ack;

  Serial.print("NEW PREAMBLE: ");
  for(int i = 0; i < PREAMBLE_SIZE; i++) {
    Serial.print(channel.preamble[i]); Serial.print("");
  }
  Serial.println("");
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

  Serial.print("PREAMBLE: ");
  for(int i = 0; i < PREAMBLE_SIZE; i++) {
    Serial.print(channel.preamble[i]); Serial.print("");
  }
  Serial.println("");
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
          receive = (*this).receive(_comm, counter++, channel);
          (*this).stopWatchStop();
          (*this).stopWatchReset();
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

      uint32_t _permittedTime = 0UL;
      (*this).stopWatchStart();

      while( _permittedTime < ( DEFAULTPERIOD + (DEFAULTPERIOD * _counter) ) && !Serial.available())
          _permittedTime = (*this).elapsed();

      /* IF THIS IS TRUE - THEN COMMUICATION FAILED - ERROR */
      if(_counter >= MAX_ATTEMPTS) { return 0; }

      Serial.println("WE HAVE A MESSAGE TO PEEK AT AND VERIFY");
      uint8_t * _response = new uint8_t[PREAMBLE_SIZE];

      for(int i = 0; i < PREAMBLE_SIZE; i++) {
        _response[i] = Serial.read();
      }

      /* IF THIS IS TRUE - THEN COMMUICATION SUCCEEDED - LETS PEEK INSIDE */
      //return (*this).peek(_comm, _response, channel);
      break;
  }
  return _return;
}

uint8_t Communicate::peek(uint8_t _comm, uint8_t * response, transmit & channel) {
  Serial.print("PEEK: ");
  for(int i = 0; i < PREAMBLE_SIZE; i++) {
    Serial.print(response[i]); Serial.print("");
  }
  Serial.println("");


  /* SWITCH TO THE RECEIVED CASE */
  switch(response[6]) {
    case SYN:
      /* IS ARDUINO ORIGINAL SYN == RECEIVED ACK */
      if(response[5] == channel.syn) {
        Serial.println("UPDATE STRUCT: ");
        channel.target = response[2];
        channel.syn = ++response[4];
        channel.ack = response[4];
        channel.discovered = true;
      }
      /* SEND FINISH MESSAGE - WE HAVE DISCOVERED AND SYNCHED */
      return 1;
      break;
    case URG:
      break;
    case RST:
      break;
    case CNT:
      break;
    case FIN:
      break;
  }
}
