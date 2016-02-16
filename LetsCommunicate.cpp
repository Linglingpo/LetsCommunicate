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

void LetsCommunicate::configureInterrupts(uint8_t _action) {
  // HARDSERIAL (RX/TX)+ DIG / DXT
  if((*this).comm_type == HARDSERIAL) {
    /* The ternary operator: condition ? expression1 : expression2
      if the condition is true, then the expression evaluates to the result of expression1;
      otherwise it evaluates to the result of expresssion2. */
    (*this).size = (_action == DIG) ? DIGSIZE : (DIGSIZE + DXTSIZE);
<<<<<<< HEAD
    //interruptStateArray(pointer to memeory) points to the new created dynamic array
    (*this).interruptStateArray = (new uint8_t [(*this).size - OFFSET]) ;
    //Assign the value in the interruptStateArray as 0
    Serial.print("Pin State: ");
    for (int i = 0; i <= (*this).size; i++){
      (*this).interruptStateArray[i] = 0;
      Serial.print((*this).interruptStateArray[i]);
    }
    Serial.println();

=======
    //offset for the RX/TX pins
>>>>>>> master
    for(int i = OFFSET; i <= (*this).size; i++) {
      pinMode(i, INPUT_PULLUP);
      //setup the boolean array for interruptState
      (*this).interruptState[i] = false ;
      /* 1st parameter: pin
      * 2nd para: user Function
      * 3rd para: mode - CHANGE(SWITCH) / RISING (HIGH)/ FALLING(LOW) */
      enableInterrupt(i, interruptHandler, CHANGE);
      Serial.print("Pin ");
      Serial.print(i);
      Serial.print(" = input");
      Serial.println();
    }
  }
}

/* ASSUMES INITIALISE WITH DIG & ANA */
void LetsCommunicate::initialiseInputWith(uint8_t _a, uint8_t _b) {
  initialiseInputAs(_a);
  initialiseInputAs(_b);
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
<<<<<<< HEAD
=======
    case ALL:
      // HMMM. May not make sense here ? why ?
    break;
>>>>>>> master
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
    delay(25);
    if(digitalRead(interrupt_id) == LOW) {
      //if it is low, the value of the interrupt State Array is 1
      (*this).interruptStateArray[interrupt_id] = 1;
      Serial.print("Actived: ");
    } else if(digitalRead(interrupt_id) == HIGH){
      (*this).interruptStateArray[interrupt_id] = 0;
      Serial.print("Deactived: ");
      }

<<<<<<< HEAD
    Serial.print(interrupt_id);
    Serial.print(" = ");

    for (int i = OFFSET; i <= (*this).size; i++){
      Serial.print((*this).interruptStateArray[i]);
    }

    Serial.println();
=======
      Serial.println(interrupt_id);
>>>>>>> master
      // need to debounce
      delay(500);
      interrupted = false;
      previousInterrupt_id = interrupt_id;
      interrupt_id = -1;
  }


  if((*this).action) {
<<<<<<< HEAD
    for(int i = 0; i < ANASIZE; i++) {

      Serial.print("Analog "); Serial.print(i); Serial.print(" Read ");
      Serial.print(analogRead(i)); Serial.println(" ");
      anaReadTempNumber = analogRead(i);
      uint8_t number_1 = anaReadTempNumber >> 8;
      uint8_t number_2 = anaReadTempNumber;
      Serial.print("anaReadTempNumber "); Serial.print(anaReadTempNumber); Serial.println(" ");
      Serial.print("number 1 "); Serial.print(number_1); Serial.println(" ");
      Serial.print("number 2 "); Serial.print(number_2); Serial.println(" ");
      uint16_t together = number_1 << 8;
      together += number_2;
      Serial.print("Together "); Serial.print(together); Serial.println(" ");

=======
    for(int i = 0; i < 6; i++) {
      //Serial.print("Analog "); Serial.print(i); Serial.print(" ");
      //Serial.print(analogRead(i)); Serial.println(" ");
>>>>>>> master
    }
    delay(1000);
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
