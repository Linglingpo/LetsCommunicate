#include "LetsCommunicate.h"

void LetsCommunicate::interruptFunction() {
  interrupt_id = arduinoInterruptedPin;
  interrupted = true;
//  if(digitalRead(interrupt_id) == LOW) {
//    interrupted = true;
//  }
}

void LetsCommunicate::status() {}

void LetsCommunicate::initConfiguration(uint8_t _flag) {
Serial.println((*this).comm_type);
Serial.println((*this).source);
Serial.println((*this).target);
Serial.println((*this).action);

if((*this).comm_type == HARDSERIAL) {
// We know Digital 0 & 1 are non USE.... HARD TX/RX
  switch((*this).action) {
    case DIG:
//CONSTRUCTOR 1
    if(_flag == 1){
      (*this).flag = _flag;
      (*this).size = DIGSIZE;
      for(int i = OFFSET; i <= (*this).size; i++){
      pinMode(i, INPUT);
      /*
       Serial.print("Pin ");
       Serial.print(i);
       Serial.print(" = input");
       Serial.println();
       */
    }
  }
  //CONSTRUCTOR 2
   if (_flag == 2){
     (*this).flag = _flag;
     //(*this).size = size;
        for(int i = 0; i < (*this).size; i++){
          if ((*this).configIO[i] == 1){
            pinMode(i + OFFSET, INPUT);
             Serial.print("Pin ");
             Serial.print(i+ OFFSET);
             Serial.print(" = input");
             Serial.println();
          }else {
            pinMode(i + OFFSET, OUTPUT);
             Serial.print("Pin ");
             Serial.print(i+OFFSET, OUTPUT);
             Serial.print(" = output");
             Serial.println();
          }
      }
    }
    break;

    case DXT:
    Serial.println("DXT");
    break;
    case ANA:
    Serial.println("ANA");
    break;
    case ALL:
    Serial.println("ALL");
    break;
  }
}
}


void LetsCommunicate::run() {
  if(interrupted) {

    if(digitalRead(interrupt_id) == LOW) {
      Serial.print("Actived: ");
    } else {
      Serial.print("Deactived: ");
    }

    Serial.println(interrupt_id);
      // need to debounce
      delay(250);
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
  (*this).pinState();
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
