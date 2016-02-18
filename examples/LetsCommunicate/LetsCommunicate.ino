#include <LingLingPo.h>
LetsCommunicate first;

/* DEFINES */
#define MASTERCOMM HARDSERIAL // (*this).Arduino -> Computer via USB | BLUETOOTH | WIFI
#define INTERCOMM  ISQUAREDC  // (*this).Arduino -> (*another).Arduino via I2C | SOFTSERIAL
#define THISARDUINOID 0
#define ANOTHERARDUINOID 1 // May be more than 1 other Arduino if using INTERCOM == I2C
#define COMPUTERID 2

// uint8_t *analogStateArray;
// uint8_t analogStateSize;
const letscommunicate * state = new letscommunicate();

//uint8_t configIO[3] {0, 1, 0};

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
  //first.initialiseInputWithInterruptsAs(DIG);
  //first.initialiseInputWithInterruptsAs(DXT);
  first.initialiseInputWithInterruptsAs(ALL);
  first.stateOfTheUnion();
}

void loop() {
  first.run();
  state = first.getLetsCommunicateState();
  Serial.println(state->digitalPinsAllocatedNonCustom);
  Serial.println(state->analogPinsAllocatedNonCustom);
  // state->digitalPinsAllocatedNonCustom = 0; // MUST GET AN ERROR - COMPILE ERROR !!!!
  //pointer
//  analogStateArray = first.getAnalogStateArray();
//  analogStateSize = first.getAnalogStateArraySize();
//  state = first.getLetsCommunicateState();
//
//      uint8_t count = 0;
//    for(int i = 0; i <analogStateSize; i+= 2) {
//      uint16_t together = analogStateArray[i] << 8;
//      together += analogStateArray[i + 1];
//      Serial.print("Ana #"); Serial.print(count++); Serial.println();
//      Serial.print("Together "); Serial.print(together); Serial.println(" ");
//    }
//
//  Serial.print("Array Size in Arduino:");
//  Serial.print(analogStateSize);
//  Serial.println();
  first.stateOfTheUnion();
  delay(100);
}
