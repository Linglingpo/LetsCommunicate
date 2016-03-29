#include "Communicate.h"

Communicate::Communicate() { }

Communicate::Communicate(uint8_t _mastercomm, uint8_t _this_id) {
  //need "new" when it is pointer
  communicationState = new communicationType();
  (*this).communicationState->mastercomm = _mastercomm;
  (*this).communicationState->source = _this_id;
}

Communicate::Communicate(uint8_t _mastercomm, uint8_t _this_id, uint8_t _intercomm) {
  new Communicate(_mastercomm, _this_id);
  (*this).communicationState->intercomm = _intercomm;
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

    case CNT:
      channel.preamble[6] = _type;
    break;

    case FIN:
      channel.preamble[6] = _type;
    break;
  }

  Serial.print("SENT PREAMBLE: ");
  for(int i = 0; i < PREAMBLE_SIZE; i++) {
    Serial.print(channel.preamble[i]); Serial.print(" ");
  }
  Serial.println("");

  return 1;
}

uint8_t Communicate::discover(uint8_t _comm) {
  uint8_t _return = 0;
  uint8_t seq = 0;
  uint8_t sequence[2] = { SYN, FIN };

  switch(_comm) {
    case HARDSERIAL:

      uint8_t receive;
      do {
        (*this).constructPreamble(_comm, (*this).communicationState->source, sequence[seq++], (*this).communicationState->master);
        receive = send(_comm, (*this).communicationState->master);
        //to control SYN & FIN in discover
        if(receive == 0) { return 0; }
      } while(seq < 2);
      return 1;
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

uint8_t Communicate::share(uint8_t _comm, uint8_t _type, uint8_t _payloadSize, uint8_t * _payloadState) {
  uint8_t _return     = 0;
  uint8_t seq         = 0;
  uint8_t sequence[3] = { SYN, CNT, FIN };

  switch(_comm) {

    case HARDSERIAL:
      /* PREPARE THE PAYLOAD and ASSOCIATE WITH CHANNEL */
      switch(_type) {
        case DIG:
          (*this).communicationState->master.payload = new uint8_t[ _payloadSize + DIGDXT_OFFSET ];
          (*this).communicationState->master.payload[0] = _type;
          (*this).communicationState->master.payload[1] = _payloadSize;

          for(int i = 0; i < _payloadSize; i++){
            (*this).communicationState->master.payload[i + DIGDXT_OFFSET] = _payloadState[i];
          }

        break;
        case DXT:
          (*this).communicationState->master.payload = new uint8_t[ _payloadSize + DIGDXT_OFFSET ];
          (*this).communicationState->master.payload[0] = _type;
          (*this).communicationState->master.payload[1] = _payloadSize;

          for(int i = 0; i < _payloadSize; i++)
            (*this).communicationState->master.payload[i + DIGDXT_OFFSET] = _payloadState[i];
        break;
      }

      uint8_t receive;
      do {
          (*this).constructPreamble(_comm, (*this).communicationState->source, sequence[seq++], (*this).communicationState->master);
          receive = send(_comm, (*this).communicationState->master);
          if(receive == 0) { return 0; }
      } while(seq < 3);
      return 1;
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

void Communicate::slip(uint8_t s) {
  if(s == 10) { //'/n'
    //Serial.println("IN SLIP ------------");
    Serial.write('!'); // 33
    Serial.write( s - 1 );
    Serial.write('?'); // 63
  } else
    Serial.write(s);
}

uint8_t Communicate::send(uint8_t _comm, transmit & channel) {
  uint8_t _return = 0;

  switch(_comm) {
    case HARDSERIAL:
      //(*this).discover(_comm);
      uint8_t counter = 0;
      uint8_t receive = 0;
      do {
          //ALWAYS TRANSMIT THE PREAMBLE {SYN, CNT or FIN}
          for(int i = 0; i < PREAMBLE_SIZE; i++) {
            (*this).slip(channel.preamble[i]);
            //delay(5);
          }

          //CNT
          if(channel.preamble[6] == CNT) {
            for(int i = 0; i < 14; i++) {
              (*this).slip((*this).communicationState->master.payload[i]);
              //delay(5);
            }
          }

          Serial.write('\n');
          receive = (*this).receive(_comm, counter++, channel);
          /* IF THIS IS TRUE - THEN COMMUICATION FAILED - ERROR - receive has several error codes */
          if( counter == MAX_ATTEMPTS ) {
            return 0;
          }
    } while ( counter != MAX_ATTEMPTS && receive != 1);

      return 1;
      break;
  }
  return _return;
}


uint8_t Communicate::receive(uint8_t _comm, uint8_t _counter, transmit & channel) {
  uint8_t _return = 0;

  switch(_comm) {
    case HARDSERIAL:
      (*this).stopWatchStop();
      (*this).stopWatchReset();
      (*this).stopWatchStart();
      uint32_t timer = 0UL;
      uint16_t _permittedTime = ( DEFAULTPERIOD + (DEFAULTPERIOD * _counter) );

      do {
        timer = (*this).elapsed();
        /* IF THIS IS TRUE - THEN COMMUICATION ATTEMPT TIMED OUT */
        if(timer >= _permittedTime) { return 0; }
      } while( timer < _permittedTime && !Serial.available());

      /* IF THIS IS TRUE - THEN COMMUICATION SUCCEEDED - LETS PEEK INSIDE */
      return (*this).peek(_comm, channel);
      break;
  }
  return _return;
}

uint8_t Communicate::peek(uint8_t _comm, transmit & channel) {

  uint8_t * response = new uint8_t[PREAMBLE_SIZE];
  Serial.print("RECIEVED PREAMBLE: ");
  for(int i = 0; i < PREAMBLE_SIZE; i++) {
    response[i] = Serial.read();
    delay(2);
  }

  for(int i = 0; i < PREAMBLE_SIZE; i++) {
    Serial.print(response[i]); Serial.print(" ");
    //delay(5);
  }
  Serial.println(" ");
  Serial.println(" ");

  if(response[0] != HELLO) { return 0; }
  if(response[1] != PREAMBLE_SIZE) { return 0; }
  if(response[3] != (*this).communicationState->source) { return 0; }
  if(response[5] != channel.syn) { return 0; }
  if(response[6] != channel.preamble[6]) { return 0; }

  if(channel.syn > 255) channel.syn = 0;

  /* SWITCH TO THE RECEIVED TYPE MESSAGE */
  switch(response[6]) {
    case SYN:
      /* WE NEED TO CHECK EVERY FIELD IN THE RECEIVED MESSAGE */
        channel.target = response[2];
        channel.ack = channel.syn;
        channel.syn = ( !channel.discovered ) ? channel.syn = response[4] + 1 : ++channel.syn;
      /* SEND FINISH MESSAGE - WE HAVE DISCOVERED AND SYNCHED */
      return 1;
      break;

    case URG:
      break;

    case RST:
      break;

    case CNT:
      channel.target = response[2];
      channel.ack = channel.syn;
      channel.syn = ++channel.syn;
      return 1;
      break;

    case FIN:
      channel.ack = channel.syn;
      channel.syn = ++channel.syn;
      channel.discovered = true;
      return 1;
      break;
  }
}
