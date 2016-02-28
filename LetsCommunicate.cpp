#include "LetsCommunicate.h"

//void LetsCommunicate::status() {}

/* INTERRUPT GLOBALLY SCOPED VARIABLES */
// For interrupt pin numbers
volatile uint8_t interrupt_id = -1;
volatile uint8_t previousInterrupt_id = -1;
volatile uint8_t interrupted = false;
/* INTERRUPT GLOBALLY SCOPED VARIBLES */

void interruptHandler() {
  interrupt_id = arduinoInterruptedPin;
  interrupted = true;
}

/* INIT: DIG, DXT, ANA - ALLOCATE ALL PINS AS INPUT PINS (NO CUSTOM SETS) */
//Not Interrupts
void LetsCommunicate::initialiseInputAs(uint8_t _payloadType) {
  (*this).selectAndInitialiseInputAs(_payloadType, false);
}

//Interrupts
void LetsCommunicate::initialiseInputWithInterruptsAs(uint8_t _payloadType) {
  (*this).selectAndInitialiseInputAs(_payloadType, true);
}

void LetsCommunicate::initialiseDIGDXT(uint8_t _payloadType) {
  // The ternary operator: condition ? expression1 : expression2
  (*this).state->digitalPinsAllocatedNonCustom = (_payloadType == DIG) ? DIGSIZE - OFFSET : (DIGSIZE + DXTSIZE) - OFFSET;
  (*this).state-> digitalPinsSize = (*this).state->digitalPinsAllocatedNonCustom;
  //readDigitalRead = array to keep the state of IO
  (*this).state->readDigitalRead = new uint8_t[(*this).state->digitalPinsAllocatedNonCustom];

  for(int i = 0; i < (*this).state->digitalPinsAllocatedNonCustom; i++) {
    //no interrupt
    pinMode(OFFSET + i, INPUT_PULLUP);
    (*this).state->readDigitalRead[i] = 0;
    //interrupt
    /* 1st parameter: pin
    * 2nd para: user Function
    * 3rd para: mode - CHANGE(SWITCH) / RISING (HIGH)/ FALLING(LOW) */
    if((*this).state->interruptsEnabled) {
      enableInterrupt(OFFSET + i, interruptHandler, CHANGE);
      //set the interrupt state as 0 long
      //(*this).state->interruptState = 0l;
    }
  }
}

/* action [] = {DIG, DXT, ANA} */
void LetsCommunicate::selectAndInitialiseInputAs(uint8_t _payloadType, bool _interruptFlag) {

  /* SET STATE FOR THE LETS COMMUNICATE SYSTEM */
  (*this).state->interruptsEnabled = _interruptFlag;
  (*this).state->customConfigOfInputOut = false;

  switch(_payloadType) {
    case DIG:
      (*this).state->payloadType[0] = true;
      if((*this).state->mastercomm == HARDSERIAL) {
        (*this).initialiseDIGDXT(_payloadType);
      }
    break;
    case DXT:
      (*this).state->payloadType[1] = true;
      if((*this).state->mastercomm == HARDSERIAL) {
        (*this).initialiseDIGDXT(_payloadType);
      }
    break;
    case ANA:
      (*this).state->payloadType[2] = true;
      (*this).state->analogPinsAllocatedNonCustom = (ANASIZE * OFFSET);
      (*this).state-> analogPinsSize = (*this).state->analogPinsAllocatedNonCustom/2;
      (*this).state->readAnalogRead = new uint8_t[(*this).state->analogPinsAllocatedNonCustom];
      for(int i = 0; i < (*this).state->analogPinsAllocatedNonCustom; i++){
      (*this).state->readAnalogRead[i] = 0;
    }
    break;
    case ALL:
      (*this).state->payloadType[0] = true;
      (*this).state->payloadType[2] = true;
      (*this).state->analogPinsAllocatedNonCustom = (ANASIZE * OFFSET);
      (*this).state-> analogPinsSize = (*this).state->analogPinsAllocatedNonCustom/2;
      (*this).state->readAnalogRead = new uint8_t[(*this).state->analogPinsAllocatedNonCustom];
      for(int i = 0; i < (*this).state->analogPinsAllocatedNonCustom; i++){
      (*this).state->readAnalogRead[i] = 0;
    }
      if((*this).state->mastercomm == HARDSERIAL) {
        (*this).initialiseDIGDXT(DIG);
      }
    break;
  }
}

//Set & Get
//Does the copy of letsCommunicate struct
const letscommunicate * LetsCommunicate::getLetsCommunicateState() const {
  return (*this).state;
};


void LetsCommunicate::stateOfTheUnion() {
  /* PRINT PUT THE CURRENT STATE OF THE SYSTEM - USED FOR QUICK OVERVIEW */
  Serial.println("*--------------------------------------------------------------------------------");
  Serial.print("* Communication Type: "); Serial.println((*this).state->mastercomm);
  Serial.print("* Source ID: "); Serial.println((*this).state->source);
  Serial.print("* Interrupts Enabled: "); Serial.println((*this).state->interruptsEnabled);
  Serial.println("* Initialised Inputs: ");
  Serial.print("DIG: "); Serial.print((*this).state->payloadType[0]); Serial.print(" DXT: "); Serial.print((*this).state->payloadType[1]);
  Serial.print(" ANA: "); Serial.println((*this).state->payloadType[2]);

  // ANA
  if((*this).state->payloadType[2]) {
  Serial.println("Analog Pins Input State: ");
  uint8_t count = 0;
    for(int i = 0; i < (*this).state->analogPinsAllocatedNonCustom; i+= 2) {
      uint16_t together = (*this).state->readAnalogRead[i] << 8;
      together += (*this).state->readAnalogRead[i + 1];
      Serial.print("Ana #"); Serial.print(count++); Serial.println();
      Serial.print("Together "); Serial.print(together); Serial.println(" ");
    }
  }
  //DIG or DXT
  if((*this).state->payloadType[0] || (*this).state->payloadType[1]) {
  Serial.println("Digital Pins Input State: ");
    for(int i = 0; i < (*this).state->digitalPinsAllocatedNonCustom; i++) {
      Serial.print((*this).state->readDigitalRead[i]); Serial.print(" ");
    }
    Serial.println(" ");
  }

/*
  // Send preamble MSG
Serial.print("This is the preamble from send in Union: ");
  for(int i = 0; i < PREAMBLE_SIZE; i++) {
    Serial.print((*this).preambleFromCommunicate[i]);
    Serial.print(" ");
  }
  Serial.println();
  */

  Serial.println("*--------------------------------------------------------------------------------");

}

/* FUNTION TO CONSTRUCT PREABLE + DATA MSG AND TRANSMIT (SEND) */
void LetsCommunicate::transmit(uint8_t _comm, uint8_t _payloadType){
    // call transmission function from Communicate.cpp
    switch(_payloadType) {
      case DIG:
      (*this).transmissionMsg(_comm, _payloadType, (*this).state-> digitalPinsSize, (*this).state->readDigitalRead);
      break;
      case DXT:
      (*this).transmissionMsg(_comm, _payloadType, (*this).state-> digitalPinsSize, (*this).state->readDigitalRead);
      break;
      case ANA:
      (*this).transmissionMsg(_comm, _payloadType, (*this).state-> analogPinsSize, (*this).state->readAnalogRead);
      break;
      case ALL:
      //???
      break;
    }
}


void LetsCommunicate::run() {
//when using interrupt (only for digital)
  if((*this).state->interruptsEnabled) {
    delay(25);
    if(digitalRead(interrupt_id) == LOW) {
      (*this).state->readDigitalRead[interrupt_id - OFFSET] = 1;
    } else if(digitalRead(interrupt_id) == HIGH){
      (*this).state->readDigitalRead[interrupt_id - OFFSET] = 0;
    }
    interrupted = false;
    interrupt_id = -1;
  }

  if(( (*this).state->payloadType[0] || (*this).state->payloadType[1] ) && !(*this).state->interruptsEnabled) {
    for(int i = 0; i < (*this).state->digitalPinsAllocatedNonCustom; i++) {
      uint8_t _digitalPin = digitalRead(i + OFFSET);
      if(_digitalPin == LOW) {
        (*this).state->readDigitalRead[i] = 1;
      } else {
        (*this).state->readDigitalRead[i] = 0;
      }
    }
  }

  if((*this).state->payloadType[2]) {
    uint8_t count = 0;
    for(int i = 0; i < (*this).state->analogPinsAllocatedNonCustom; i+= 2) {
      uint16_t anaReadTempNumber = analogRead(count);
      count ++;
      //Serial.print("Analog Read: "); Serial.println(anaReadTempNumber);
      (*this).state->readAnalogRead[i] = anaReadTempNumber >> 8;
      (*this).state->readAnalogRead[i + 1] = anaReadTempNumber;
      delay(5);
    }
  }
}
