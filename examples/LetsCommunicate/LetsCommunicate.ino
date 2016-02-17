#include <LingLingPo.h>
LetsCommunicate first;

/* DEFINES */
#define MASTERCOMM HARDSERIAL // (*this).Arduino -> Computer via USB | BLUETOOTH | WIFI
#define INTERCOMM  ISQUAREDC  // (*this).Arduino -> (*another).Arduino via I2C | SOFTSERIAL
#define THISARDUINOID 0
#define ANOTHERARDUINOID 1 // May be more than 1 other Arduino if using INTERCOM == I2C
#define COMPUTERID 2

uint8_t configIO[3] {0, 1, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //first = LetsCommunicate();
  first = LetsCommunicate(MASTERCOMM, THISARDUINOID, COMPUTERID, INTERCOMM);
  /* TEST THESE CONFIGS - ONE AT A TIME */
  //first.initialiseInputAs(DIG);
  first.initialiseInputWithInterruptsAs(DXT);
  //first.initialiseInputAs(DXT);
  //first.initialiseInputAs(ANA);
  first.stateOfTheUnion();
}

void loop() {
  first.run();
  first.stateOfTheUnion();
  delay(2000);
}
