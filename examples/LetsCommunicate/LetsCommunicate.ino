#include <LetsCommunicate.h>
LetsCommunicate first;

boolean pinStates[3] {0};
const int buttonPins[3] {2, 3, 4};

int buttonPin = 2;
int buttonState = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i; i < 4; i++) {
    pinMode(buttonPins[i], INPUT);
  }

  first = LetsCommunicate();
}

void loop() {

  for (int i; i < 4; i++) {
    pinStates[i] = digitalRead(buttonPins [i]);
  }

  if (pinStates[2] == HIGH) {
    first.pinActive (buttonPins[2]);
    //first.run();
  }
  delay(500);
}
