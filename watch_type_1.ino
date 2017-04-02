#include <Arduino.h>
#include "program.h"

Program program = Program();

void timer1_interrupt() {
  program.timer1_interrupt();
}

void timer2_interrupt() {
  program.timer2_interrupt();
}

void setup() {
  Serial.begin(115200);
  
  program.setup();
  program.attach_timer1_interrupt(timer1_interrupt);
  program.attach_timer2_interrupt(timer2_interrupt);
  program.start_timer1();
  //program.start_timer2();
}

void loop() {
  program.loop();
}
