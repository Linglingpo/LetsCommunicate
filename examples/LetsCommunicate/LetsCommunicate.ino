#include <LingLingPo.h>
LetsCommunicate first;

/* DEFINES */
#define MASTERCOMM HARDSERIAL // (*this).Arduino -> Computer via USB | BLUETOOTH | WIFI
#define INTERCOMM  ISQUAREDC  // (*this).Arduino -> (*another).Arduino via I2C | SOFTSERIAL
#define THISARDUINOID 1
/* THESE ARE DETERMINED THROUGH THE DISCOVERY PHASE - THREE WAY HANDSHAKE */
//#define ANOTHERARDUINOID 1 // May be more than 1 other Arduino if using INTERCOM == I2C
//#define COMPUTERID 2

uint8_t *digitalStateArray;
int pinNum = 2;
int pinState = 0;

//must to be const
const letscommunicate * state = new letscommunicate();

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  Serial.begin(57600);
  /* ARDUINO -> PROCESSING CONFIGURATION */
  first = LetsCommunicate(MASTERCOMM, THISARDUINOID);

  /* ARDUINO -> PROCESSING CONFIGURATION && ARDUINO <-> DEVICES */
  //first = LetsCommunicate(MASTERCOMM, THISARDUINOID, INTERCOMM);

  /* TEST THESE CONFIGS - ONE AT A TIME */
  //first.initialiseInputAs(DIG);
  //first.initialiseInputAs(DXT);
  //first.initialiseInputAs(ANA);
  //first.initialiseInputAs(ALL);
  first.initialiseInputWithInterruptsAs(DIG);
  //first.initialiseInputWithInterruptsAs(DXT);
  //first.initialiseInputWithInterruptsAs(ALL);
  first.stateOfTheUnion();
  /* DISCOVER MASTER TARGET */
  if(!first.discover(MASTERCOMM)) { Serial.println("Check Connections to TARGET Device | Program - Currently Not Available! "); }
  /* DISCOVER INTER TARGET(s) */
  //if(!first.discover(INTERCOMM)) { Serial.println("Check Connections to TARGET Device | Program - Currently Not Available! "); }
}

void loop() {
  first.run();
  state = first.getLetsCommunicateState();
  //first.stateOfTheUnion();
  first.transmit(MASTERCOMM, DIG);
  //first.transmit(MASTERCOMM, DXT);
  //first.transmit(MASTERCOMM, ANA);

  //first.send(MASTERCOMM);

  //state->digitalPinsAllocatedNonCustom = 0; // MUST GET AN ERROR - COMPILE ERROR !!!!

  //first.stateOfTheUnion();
  delay(500);
}
