#include <Arduino.h>
#include "program.h"

Program program = Program();

void setup() {
  program.setup();
}

void loop() {
  program.loop();
}
