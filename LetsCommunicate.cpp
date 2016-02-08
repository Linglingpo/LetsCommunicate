#include "LetsCommunicate.h"

void LetsCommunicate::status() {}

/* INTERRUPT GLOBALLY SCOPED VARIABLES */
volatile uint8_t interrupt_id = -1;
volatile uint8_t previousInterrupt_id = -1;
volatile uint8_t interrupted = false;
/* INTERRUPT GLOBALLY SCOPED VARIBLES */

void interruptHandler() {
  interrupt_id = arduinoInterruptedPin;
  interrupted = true;
}

void LetsCommunicate::configureInterrupts(uint8_t _action) {

  if((*this).comm_type == HARDSERIAL) {
    (*this).size = (_action == DIG) ? DIGSIZE : (DIGSIZE + DXTSIZE);
    for(int i = OFFSET; i <= (*this).size; i++) {
      pinMode(i, INPUT_PULLUP);
      enableInterrupt(i, interruptHandler, CHANGE);
      Serial.print("Pin ");
      Serial.print(i);
      Serial.print(" = input");
      Serial.println();
    }
  }
}

/* ASSUMES INITIALISE AS DIG */
void LetsCommunicate::initialiseInputAs(uint8_t _action) {

  switch(_action) {
    case DIG:
      (*this).configureInterrupts(_action);
    break;
    case DXT:
      (*this).configureInterrupts(_action);
    break;
    case ANA:
      // USE (*this).ANA TO DETERMINE HOW TO READ FROM ANALOG PINS.
      (*this).action = 1;
    break;
    case ALL:
      // HMMM. May not make sense here ?
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


void LetsCommunicate::run() {
  if(interrupted) {

    if(digitalRead(interrupt_id) == LOW) {
      Serial.print("Actived: ");
    } else {
      Serial.print("Deactived: ");
    }

    Serial.println(interrupt_id);
      // need to debounce
      delay(125);
      interrupted = false;
      previousInterrupt_id = interrupt_id;
      interrupt_id = -1;
//    if((previousInterrupt_id != interrupt_id) || (previousInterrupt_id == interrupt_id && digitalRead(interrupt_id) == LOW)) {
//      if(previousInterrupt_id != interrupt_id) {
//      Serial.print("Actived A: ");
//      } else {
//        Serial.print("Actived B: ");
//      }
//      Serial.println(interrupt_id);
//      // need to debounce
//      delay(250);
//      interrupted = false;
//      previousInterrupt_id = interrupt_id;
//      interrupt_id = -1;
//    }
  }

  if((*this).action) {
    for(int i = 0; i < 6; i++) {
      Serial.print("Analog "); Serial.print(i); Serial.print(" ");
      Serial.print(analogRead(i)); Serial.println(" ");
    }
  }
}

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


void LetsCommunicate::pinActiveProsses(uint8_t _pinNumber) {
//do something when active
}
