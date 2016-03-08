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
//Not Interrupts - DIX/DXT/ANA/ALL
void LetsCommunicate::initialiseInputAs(uint8_t _payloadType) {
  (*this).initializeInputTypeAndReadPinsState(_payloadType, false);
}

//Interrupts - DIX/DXT/ALL
void LetsCommunicate::initialiseInputWithInterruptsAs(uint8_t _payloadType) {
  (*this).initializeInputTypeAndReadPinsState(_payloadType, true);
}

void LetsCommunicate::initialiseDIGDXT(uint8_t _payloadType) {
  // The ternary operator: condition ? expression1 : expression2
  (*this).state->digitalPinCount = (_payloadType == DIG) ? DIGSIZE - OFFSET : (DIGSIZE + DXTSIZE) - OFFSET;
  //presentDigitalState = array to keep the state of IO
  (*this).state->presentDigitalState = new uint8_t[(*this).state->digitalPinCount];

  for(int i = 0; i < (*this).state->digitalPinCount; i++) {
    //no interrupt
    pinMode(OFFSET + i, INPUT_PULLUP);
    (*this).state->presentDigitalState[i] = 0;
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

/* INITIALISE THE INPUT BY PAYLOAD TYPES (DIX/DET/ANA/ALL) AND READ THE STATE OF EACH PIN */
void LetsCommunicate::initializeInputTypeAndReadPinsState(uint8_t _payloadType, bool _interruptFlag) {

  /* SET STATE FOR THE LETS COMMUNICATE SYSTEM */
  (*this).state->interruptsEnabled = _interruptFlag;
  (*this).state->customConfigOfInputOut = false;

  switch(_payloadType) {
    case DIG:
      (*this).state->payloadType[0] = true;
      //HARDSERIAL
      if((*this).state->mastercomm == HARDSERIAL) {
        (*this).initialiseDIGDXT(_payloadType);
      }// end of HARDSERIAL
    break;
    case DXT:
      (*this).state->payloadType[1] = true;
      //HARDSERIAL
      if((*this).state->mastercomm == HARDSERIAL) {
        (*this).initialiseDIGDXT(_payloadType);
      }// end of HARDSERIAL
    break;
    case ANA:
    //HARDSERIAL
      if((*this).state->mastercomm == HARDSERIAL) {
      (*this).state->payloadType[2] = true;
      (*this).state->analogPinCount = ANASIZE;
      (*this).state->analogDataSize = ((*this).state->analogPinCount * ANALOG_OFFSET);
      (*this).state->presentAnalogState = new uint8_t[(*this).state->analogDataSize];
      for(int i = 0; i < (*this).state->analogDataSize; i++){
      (*this).state->presentAnalogState[i] = 0;
        }
      }// end of HARDSERIAL
    break;
    case ALL:
    //HARDSERIAL
      if((*this).state->mastercomm == HARDSERIAL) {
      (*this).state->payloadType[0] = true;
      (*this).state->payloadType[2] = true;
      (*this).state->analogPinCount = ANASIZE;
      (*this).state->analogDataSize = ((*this).state->analogPinCount * ANALOG_OFFSET);
      (*this).state->presentAnalogState = new uint8_t[(*this).state->analogDataSize];
      for(int i = 0; i < (*this).state->analogDataSize; i++){
      (*this).state->presentAnalogState[i] = 0;
        }
      (*this).initialiseDIGDXT(DIG);
      }// end of HARDSERIAL
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
    for(int i = 0; i < (*this).state->analogDataSize; i+= 2) {
      uint16_t together = (*this).state->presentAnalogState[i] << 8;
      together += (*this).state->presentAnalogState[i + 1];
      Serial.print("Ana #"); Serial.print(count++); Serial.println();
      Serial.print("Together "); Serial.print(together); Serial.println(" ");
    }
  }
  //DIG or DXT
  if((*this).state->payloadType[0] || (*this).state->payloadType[1]) {
  Serial.println("Digital Pins Input State: ");
    for(int i = 0; i < (*this).state->digitalPinCount; i++) {
      Serial.print((*this).state->presentDigitalState[i]); Serial.print(" ");
    }
    Serial.println(" ");
  }

  //------  MSG to transmit -------//
  Serial.println("Transmit Massage: ");
  //ANA
  if((*this).state->payloadType[2]){
    //???
  }
  //DIG or DXT
  if((*this).state->payloadType[0] || (*this).state->payloadType[1]){
  for(int i = 0; i < (*this).transmitTotalMsgSize; i++){
    Serial.print(masterMsg[i]);
    Serial.print(" ");
    }
  }
  // All ???

  Serial.println();
  Serial.println("*--------------------------------------------------------------------------------");

}

/* FUNTION TO CONSTRUCT PREABLE + DATA MSG AND TRANSMIT (SEND) */
// NEED TO WORK ON IT - OVERRIDE transmit with diff parameters from the one in communicate calss
// CHECK THE FLAG THAT IF THERE IS DATA READY TO TRANSMIT
void LetsCommunicate::transmit(uint8_t _comm, uint8_t _payloadType){
    // call transmission function from Communicate.cpp
    switch(_payloadType) {
      case DIG:
      (*this).transmissionMsg(_comm, _payloadType, (*this).state-> digitalPinCount, (*this).state->presentDigitalState);
      break;
      case DXT:
      (*this).transmissionMsg(_comm, _payloadType, (*this).state-> digitalPinCount, (*this).state->presentDigitalState);
      break;
      case ANA:
      break;
      case ALL:
      break;
    }
}


void LetsCommunicate::run() {
//when using interrupt (only for digital)
// check current state and pre state ...
  if((*this).state->interruptsEnabled) {
    delay(25);
    if(digitalRead(interrupt_id) == LOW) {
      (*this).state->presentDigitalState[interrupt_id - OFFSET] = 1;
    } else if(digitalRead(interrupt_id) == HIGH){
      (*this).state->presentDigitalState[interrupt_id - OFFSET] = 0;
    }
    interrupted = false;
    interrupt_id = -1;
  }

  if(( (*this).state->payloadType[0] || (*this).state->payloadType[1] ) && !(*this).state->interruptsEnabled) {
    for(int i = 0; i < (*this).state->digitalPinCount; i++) {
      uint8_t _digitalPin = digitalRead(i + OFFSET);
      if(_digitalPin == LOW) {
        (*this).state->presentDigitalState[i] = 1;
      } else {
        (*this).state->presentDigitalState[i] = 0;
      }
    }
  }

  if((*this).state->payloadType[2]) {
    uint8_t count = 0;
    for(int i = 0; i < (*this).state->analogDataSize; i+= 2) {
      uint16_t anaReadTempNumber = analogRead(count);
      count ++;
      //Serial.print("Analog Read: "); Serial.println(anaReadTempNumber);
      (*this).state->presentAnalogState[i] = anaReadTempNumber >> 8;
      (*this).state->presentAnalogState[i + 1] = anaReadTempNumber;
      delay(5);
    }
  }
}
