#include "LetsCommunicate.h"

void LetsCommunicate::status() { (*this).initConfiguration(); }

void LetsCommunicate::initConfiguration() {
Serial.println((*this).comm_type);
Serial.println((*this).source);
Serial.println((*this).target);
Serial.println((*this).action);

if((*this).comm_type == HARDSERIAL) {
// We know Digital 0 & 1 are non USE.... HARD TX/RX
  switch((*this).action) {
    case DIG:
      //for(int i = OFFSET; i <= DIGSIZE; i++)
        //pinMode(i, INPUT);

        for(int i = 0; i < (*this).size; i++){
          if ((*this).configIO[i] == 1){
            pinMode(i + OFFSET, INPUT);
            // Serial.print("Pin ");
            // Serial.print(i+ OFFSET);
            // Serial.print(" = input");
            // Serial.println();
          }else {
            pinMode(i + OFFSET, OUTPUT);
            // Serial.print("Pin ");
            // Serial.print(i+OFFSET, OUTPUT);
            // Serial.print(" = output");
            // Serial.println();
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
  // Waiting for something to happen...
  for(int i = 2; i <= 4; i++) {
    if(digitalRead(i) == HIGH)  {
      Serial.print("Digital Pin: ");
      Serial.println(i, DEC);
    }
  }

}

void LetsCommunicate::pinActive(uint8_t _pinNumber) {
  // Waiting for something to happen...
  Serial.print("Pin ");
  Serial.print(_pinNumber);
  Serial.print(" = high");
  Serial.println();
}
