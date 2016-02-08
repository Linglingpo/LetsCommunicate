
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>
#include <LetsCommunicate.h>
LetsCommunicate first;
//LetsCommunicate second;


uint8_t configIO[3] {0, 1, 0};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //first = LetsCommunicate(0, 2, 3, 252);
  first = LetsCommunicate(0, 2, 3, 252, configIO, 3);
}

void loop() {
  first.run();
  delay(500);
}
