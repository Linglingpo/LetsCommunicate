#include <LingLingPo.h>
LetsCommunicate first;

/* DEFINES */
#define MASTERCOMM HARDSERIAL // (*this).Arduino -> Computer via USB | BLUETOOTH | WIFI
#define INTERCOMM  ISQUAREDC  // (*this).Arduino -> (*another).Arduino via I2C | SOFTSERIAL
#define THISARDUINOID 0
#define ANOTHERARDUINOID 1 // May be more than 1 other Arduino if using INTERCOM == I2C
#define COMPUTERID 2

uint8_t *digitalStateArray;
int pinNum = 2;
int pinState = 0;

//must to be const
const letscommunicate * state = new letscommunicate();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //first = LetsCommunicate();
  first = LetsCommunicate(MASTERCOMM, THISARDUINOID, COMPUTERID, INTERCOMM);
  /* TEST THESE CONFIGS - ONE AT A TIME */
  //first.initialiseInputAs(DIG);
  //first.initialiseInputAs(DXT);
  //first.initialiseInputAs(ANA);
  //first.initialiseInputAs(ALL);
  first.initialiseInputWithInterruptsAs(DIG);
  //first.initialiseInputWithInterruptsAs(DXT);
  //first.initialiseInputWithInterruptsAs(ALL);
  //first.stateOfTheUnion();

}

void loop() {
  first.run();
  state = first.getLetsCommunicateState();
  pinState = digitalRead(pinNum);
  if (pinState == LOW) {
    //Serial.println(state->digitalPinsSize);
    for (int i = 0; i < state->digitalPinsSize; i++) {
      Serial.print(state->readDigitalRead[i]);
    }
    Serial.println();
  }

  //state->digitalPinsAllocatedNonCustom = 0; // MUST GET AN ERROR - COMPILE ERROR !!!!

  //first.stateOfTheUnion();
  //delay(500);
}
