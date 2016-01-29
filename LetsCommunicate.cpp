#include "LetsCommunicate.h"

void LetsCommunicate::status() { (*this).initConfiguration();  Serial.println((*this).elapsed()); }
