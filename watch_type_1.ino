#include <Arduino.h>
#include <avr/sleep.h>
#include "program.h"

Program program = Program();

void timer1_interrupt() {
  program.timer1_interrupt();
  sleep_disable();
}

void setup() {
  Serial.begin(115200);

  set_sleep_mode(SLEEP_MODE_STANDBY);
  ADCSRA = 0;
  
  sleep_cpu();
  
  program.setup();
  program.attach_timer1_interrupt(timer1_interrupt);
  program.start_timer1();
}

void loop() {
  program.loop();
  sleep_enable();
}
