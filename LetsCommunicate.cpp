#include "LetsCommunicate.h"

void LetsCommunicate::status() {}

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
void LetsCommunicate::initialiseInputAs(uint8_t _action) {
  (*this).selectAndInitialiseInputAs(_action, false);
}

void LetsCommunicate::initialiseInputWithInterruptsAs(uint8_t _action) {
  (*this).selectAndInitialiseInputAs(_action, true);
}

void LetsCommunicate::initialiseDIGDXT(uint8_t _action) {
  // The ternary operator: condition ? expression1 : expression2
  (*this).state->digitalPinsAllocatedNonCustom = (_action == DIG) ? DIGSIZE - OFFSET : (DIGSIZE + DXTSIZE) - OFFSET;
  (*this).state->readDigitalRead = new uint8_t[(*this).state->digitalPinsAllocatedNonCustom];

  for(int i = 0; i < (*this).state->digitalPinsAllocatedNonCustom; i++) {
    pinMode(OFFSET + i, INPUT_PULLUP);
    /* 1st parameter: pin
    * 2nd para: user Function
    * 3rd para: mode - CHANGE(SWITCH) / RISING (HIGH)/ FALLING(LOW) */
    if((*this).state->interruptsEnabled) {
      enableInterrupt(OFFSET + i, interruptHandler, CHANGE);
      (*this).state->interruptState = 0l;
    }
  }
}

//   void initialiseAndAllocate(uint8_t, bool, uint8_t *, uint8_t); // , uint8_t * _digitalConfigIOSet, uint8_t _digitalConfigIOSize
void LetsCommunicate::selectAndInitialiseInputAs(uint8_t _action, bool _interruptFlag) {

  /* SET STATE FOR THE LETS COMMUNICATE SYSTEM */
  (*this).state->interruptsEnabled = _interruptFlag;
  (*this).state->customConfigOfInputOut = false;

  switch(_action) {
    case DIG:
      (*this).state->action[0] = true;
      if((*this).state->mastercomm == HARDSERIAL) {
        (*this).initialiseDIGDXT(_action);
      }
    break;
    case DXT:
      (*this).state->action[1] = true;
      if((*this).state->mastercomm == HARDSERIAL) {
        (*this).initialiseDIGDXT(_action);
      }
    break;
    case ANA:
      (*this).state->action[2] = true;
      (*this).state->analogPinsAllocatedNonCustom = (ANASIZE * OFFSET);
      (*this).state->readAnalogRead = new uint8_t[(*this).state->analogPinsAllocatedNonCustom];
    break;
    case ALL:
      (*this).state->action[0] = true;
      (*this).state->action[2] = true;
      (*this).state->analogPinsAllocatedNonCustom = (ANASIZE * OFFSET);
      (*this).state->readAnalogRead = new uint8_t[(*this).state->analogPinsAllocatedNonCustom];
      if((*this).state->mastercomm == HARDSERIAL) {
        (*this).initialiseDIGDXT(DIG);
      }
    break;
  }
}

// /* ASSUMES INITIALISE WITH DIG & ANA */
// void LetsCommunicate::initialiseInputWith(uint8_t _a, bool _interruptFlagA, uint8_t _b, bool _interruptFlagB) {
//   initialiseInputAs(_a, _interruptFlagA);
//   initialiseInputAs(_b, _interruptFlagB);
// }

// /* ASSUMES INITIALISE AS DIG */
// void LetsCommunicate::initialiseInputAs(uint8_t _action, bool _interruptFlag, uint8_t * _digitalConfigIOSet, uint8_t _digitalConfigIOSize) {
//  // (*this).digitalConfigIO = _digitalConfigIOSet;
//  // (*this).digitalConfigIOSize = _digitalConfigIOSize;
//  // (*this).initialiseInputAs(_action, _interruptFlag);
// }




// void LetsCommunicate::digitalInterrupted(){
//   if(interruptFlag){
//   if(interrupted) {
//     delay(25);
//     if(digitalRead(interrupt_id) == LOW) {
//       (*this).interruptStateArray[interrupt_id - OFFSET] = 1;
//       Serial.print("Actived: ");
//     } else if(digitalRead(interrupt_id) == HIGH){
//       (*this).interruptStateArray[interrupt_id - OFFSET] = 0;
//       Serial.print("Deactived: ");
//       }
//     Serial.print(interrupt_id);
//     Serial.print(" = ");
//     for (int i = 0; i < (*this).totalPinSize; i++){
//       Serial.print((*this).interruptStateArray[i]);
//     }
//     Serial.println();
//       interrupted = false;
//       previousInterrupt_id = interrupt_id;
//       interrupt_id = -1;
//   }
// }
//   else {
//     Serial.print("not interrupted");
//     Serial.println();
//   }
// }

void LetsCommunicate::stateOfTheUnion() {
  /* PRINT PUT THE CURRENT STATE OF THE SYSTEM - USED FOR QUICK OVERVIEW */
  Serial.println("*--------------------------------------------------------------------------------");
  Serial.print("* Communication Type: "); Serial.println((*this).state->mastercomm);
  Serial.print("* Source ID: "); Serial.println((*this).state->source);
  Serial.print("* Target ID: "); Serial.println((*this).state->target);
  Serial.print("* Interrupts Enabled: "); Serial.println((*this).state->interruptsEnabled);
  Serial.print("* Custom Config: "); Serial.println((*this).state->customConfigOfInputOut);
  Serial.println("* Initialised Inputs: ");
  Serial.print("DIG: "); Serial.print((*this).state->action[0]); Serial.print(" DXT: "); Serial.print((*this).state->action[1]);
  Serial.print(" ANA: "); Serial.println((*this).state->action[2]);
  Serial.print("Total Number of Digital Pins Enabled: ");
  Serial.println((*this).state->digitalPinsAllocatedNonCustom);

  Serial.print("Total Number of ANALOG Pins Enabled: ");
  Serial.println((*this).state->analogPinsAllocatedNonCustom);

  Serial.print("State of the Interrupt Pins: ");
  Serial.println((*this).state->interruptState, BIN);

  if((*this).state->action[2]) {
  Serial.println("Analog Pins Input State: ");
    for(int i = 0; i < (*this).state->analogPinsAllocatedNonCustom; i+= 2) {

      uint16_t together = (*this).state->readAnalogRead[i] << 8;
      together += (*this).state->readAnalogRead[i + 1];
      Serial.print("Together "); Serial.print(together); Serial.println(" ");
    }
  }

  if((*this).state->action[0] || (*this).state->action[1]) {
  Serial.println("Digital Pins Input State: ");
    for(int i = 0; i < (*this).state->digitalPinsAllocatedNonCustom; i++) {
      Serial.print((*this).state->readDigitalRead[i]); Serial.print(" ");
    }
    Serial.println(" ");
  }

  Serial.println("*--------------------------------------------------------------------------------");
}


void LetsCommunicate::run() {
  //when using interrupt (only for digital)
//(*this).digitalInterrupted();

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

  if(( (*this).state->action[0] || (*this).state->action[1] ) && (*this).state->interruptsEnabled == false) {
    for(int i = 0; i < (*this).state->digitalPinsAllocatedNonCustom; i++) {
      uint8_t _digitalPin = digitalRead(i + OFFSET);
      if(_digitalPin == LOW) {
        (*this).state->readDigitalRead[i] = 1;
      } else {
        (*this).state->readDigitalRead[i] = 0;
      }
    }
  }

  if((*this).state->action[2]) {
    for(int i = 0; i < (*this).state->analogPinsAllocatedNonCustom; i+= 2) {
      uint16_t anaReadTempNumber = analogRead(i);
      //Serial.print("Analog Read: "); Serial.println(anaReadTempNumber);
      (*this).state->readAnalogRead[i] = anaReadTempNumber >> 8;
      (*this).state->readAnalogRead[i + 1] = anaReadTempNumber;
      delay(5);
    }
  }

}

/*
void LetsCommunicate::pinState(){
  for(int i = OFFSET; i < (*this).size; i++){
    if(digitalRead(i) == HIGH)  {
      (*this).digStateArray[i] = 1 ;
    } else {
    (*this).digStateArray[i] = 0;
  }
    Serial.print((*this).digStateArray[i]);
  }// end of for loop
  Serial.println();
}
*/


// void LetsCommunicate::pinActiveProsses(uint8_t _pinNumber) {
// //do something when active
// }
