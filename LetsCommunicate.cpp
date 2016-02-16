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

void LetsCommunicate::configureDigitalInputOutput(uint8_t _action, uint8_t * _digitalConfigIO = 0, uint8_t _digitalConfigIOSize = 0) {
  // HARDSERIAL (RX/TX)+ DIG / DXT
  if((*this).comm_type == HARDSERIAL) {
    // The ternary operator: condition ? expression1 : expression2
    (*this).totalPinSize = (_action == DIG) ? DIGSIZE - OFFSET : (DIGSIZE + DXTSIZE) - OFFSET;
    //interruptStateArray(pointer to memeory) points to the new created dynamic array
    (*this).interruptStateArray = (new uint8_t [(*this).totalPinSize]) ;
    //Assign the value in the interruptStateArray as 0
     for (int i = 0; i < (*this).totalPinSize; i++){
      (*this).interruptStateArray[i] = 0;
    }
    for(int i = 0; i < (*this).totalPinSize; i++) {
      pinMode(OFFSET + i, INPUT_PULLUP);
      //setup the boolean array for interruptState
      (*this).interruptState[i] = false;
      /* 1st parameter: pin
      * 2nd para: user Function
      * 3rd para: mode - CHANGE(SWITCH) / RISING (HIGH)/ FALLING(LOW) */
      //enableInterrupt(OFFSET + i, interruptHandler, CHANGE);
    }
  }
}

/* ASSUMES INITIALISE WITH DIG & ANA */
void LetsCommunicate::initialiseInputWith(uint8_t _a, bool _interruptFlagA, uint8_t _b, bool _interruptFlagB) {
  initialiseInputAs(_a, _interruptFlagA);
  initialiseInputAs(_b, _interruptFlagB);
}

/* ASSUMES INITIALISE AS DIG */
void LetsCommunicate::initialiseInputAs(uint8_t _action, bool _interruptFlag, uint8_t * _digitalConfigIOSet, uint8_t _digitalConfigIOSize) {
 (*this).digitalConfigIO = _digitalConfigIOSet;
 (*this).digitalConfigIOSize = _digitalConfigIOSize;
 (*this).initialiseInputAs(_action, _interruptFlag);
}

/* ASSUMES INITIALISE AS DIG */
void LetsCommunicate::initialiseInputAs(uint8_t _action, bool _interruptFlag) {
  // The ternary operator: condition ? expression1 : expression2
  //(*this).totalPinSize = (_action == DIG) ? DIGSIZE - OFFSET : (DIGSIZE + DXTSIZE) - OFFSET;
  //???
  //(*this).digitalConfigIO = {2,3,4,5,6,7,8,9,10,11,12,13};
  //(*this).digitalConfigIOSize = (*this).totalPinSize;

checkAction(_action, _interruptFlag);
}

void LetsCommunicate::checkAction(uint8_t _action, bool _interruptFlag, uint8_t * _digitalConfigIOSet = {0}, uint8_t _digitalConfigIOSize = 0){

  switch(_action) {
    case DIG:
      (*this).configureDigitalInputOutput(_action);
      (*this).interruptFlag = _interruptFlag;
    break;
    case DXT:
      (*this).configureDigitalInputOutput(_action);
      (*this).interruptFlag = _interruptFlag;
    break;
    case ANA:
      // USE (*this).ANA TO DETERMINE HOW TO READ FROM ANALOG PINS.
      (*this).action = 1;
    break;
  }
}

// void LetsCommunicate::initConfiguration(uint8_t _flag) {
// Serial.println((*this).comm_type);
// Serial.println((*this).source);
// Serial.println((*this).target);
// Serial.println((*this).action);
//
// if((*this).comm_type == HARDSERIAL) {
// // We know Digital 0 & 1 are non USE.... HARD TX/RX
//   switch((*this).action) {
//     case DIG:
// //CONSTRUCTOR 1
//     if(_flag == 1){
//       (*this).flag = _flag;
//       (*this).size = DIGSIZE;
//       for(int i = OFFSET; i <= (*this).size; i++){
//       pinMode(i, INPUT);
//       /*
//        Serial.print("Pin ");
//        Serial.print(i);
//        Serial.print(" = input");
//        Serial.println();
//        */
//     }
//   }
//   //CONSTRUCTOR 2
//    if (_flag == 2){
//      (*this).flag = _flag;
//      //(*this).size = size;
//         for(int i = 0; i < (*this).size; i++){
//           if ((*this).configIO[i] == 1){
//             pinMode(i + OFFSET, INPUT);
//              Serial.print("Pin ");
//              Serial.print(i+ OFFSET);
//              Serial.print(" = input");
//              Serial.println();
//           }else {
//             pinMode(i + OFFSET, OUTPUT);
//              Serial.print("Pin ");
//              Serial.print(i+OFFSET, OUTPUT);
//              Serial.print(" = output");
//              Serial.println();
//           }
//       }
//     }
//     break;
//
//     case DXT:
//     Serial.println("DXT");
//     break;
//     case ANA:
//     Serial.println("ANA");
//     break;
//     case ALL:
//     Serial.println("ALL");
//     break;
//   }
// }
// }
void LetsCommunicate::digitalInterrupted(){
  if(interruptFlag){
  if(interrupted) {
    delay(25);
    if(digitalRead(interrupt_id) == LOW) {
      (*this).interruptStateArray[interrupt_id - OFFSET] = 1;
      Serial.print("Actived: ");
    } else if(digitalRead(interrupt_id) == HIGH){
      (*this).interruptStateArray[interrupt_id - OFFSET] = 0;
      Serial.print("Deactived: ");
      }
    Serial.print(interrupt_id);
    Serial.print(" = ");
    for (int i = 0; i < (*this).totalPinSize; i++){
      Serial.print((*this).interruptStateArray[i]);
    }
    Serial.println();
      interrupted = false;
      previousInterrupt_id = interrupt_id;
      interrupt_id = -1;
  }
}
  else {
    Serial.print("not interrupted");
    Serial.println();
  }
}

void LetsCommunicate::run() {
  //when using interrupt (only for digital)
(*this).digitalInterrupted();

  if((*this).action) {
    for(int i = 0; i < ANASIZE; i++) {

      Serial.print("Analog "); Serial.print(i); Serial.print(" Read ");
      Serial.print(analogRead(i)); Serial.println(" ");
      anaReadTempNumber = analogRead(i);
      uint8_t number_1 = anaReadTempNumber >> 8;
      uint8_t number_2 = anaReadTempNumber;
      //Serial.print("AnalogReadNumber "); Serial.print(analogRead(i)); Serial.println(" ");
      //Serial.print("number 1 "); Serial.print(number_1); Serial.println(" ");
      //Serial.print("number 2 "); Serial.print(number_2); Serial.println(" ");
      uint16_t together = number_1 << 8;
      together += number_2;
      Serial.print("Together "); Serial.print(together); Serial.println(" ");
    delay(50);
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


void LetsCommunicate::pinActiveProsses(uint8_t _pinNumber) {
//do something when active
}
