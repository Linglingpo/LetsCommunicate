#include <LingLingPo.h>
LetsCommunicate first;

uint8_t configIO[3] {0, 1, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //first = LetsCommunicate();
  /* HARDSERIAL, to ID 2, from ID 3) */
  first = LetsCommunicate(0, 2, 3);
  /* TEST THESE CONFIGS - ONE AT A TIME */
  first.initialiseInputAs(DIG);
  //first.initialiseInputAs(DXT);
  //first.initialiseInputAs(ANA);
}

void loop() {
  first.run();
}
