#include <LingLingPo.h>
LetsCommunicate first;

uint8_t configIO[3] {0, 1, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //first = LetsCommunicate();
  first = LetsCommunicate(0, 2, 3);
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
