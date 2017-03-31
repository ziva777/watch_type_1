#include <Arduino.h>
#include <LiquidCrystal.h>
#include "button.h"
#include "timer1.h"
#include "timer2.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
uint16_t msec_counter = 0;
uint16_t msec_counter_tmp = 0;

namespace Pin {
  const uint8_t BUTTON1 = 10;
  const uint8_t BUTTON2 = 11;
  const uint8_t BUTTON3 = 12;
  const uint8_t BUTTON4 = A8;

  const uint8_t CONTRAST = 2;
  const uint8_t BRIGHTNESS = 3;
};

Button button1 = Button(Pin::BUTTON1);
Button button2 = Button(Pin::BUTTON2);
Button button3 = Button(Pin::BUTTON3);
Button button4 = Button(Pin::BUTTON4);

void timer1_callback() {
  // ++msec_counter;
}

void timer2_callback() {
  static uint16_t counter = 0;
  static uint16_t brightness = 0;

  if (counter % 100 == 0) {
    analogWrite(Pin::BRIGHTNESS, brightness);
    ++brightness;
    brightness %= 255;
  }

  ++counter;
}

void setup() {
  Serial.begin(115200);

  pinMode(Pin::CONTRAST, OUTPUT);
  pinMode(Pin::BRIGHTNESS, OUTPUT);

  analogWrite(Pin::CONTRAST, 55);
  analogWrite(Pin::BRIGHTNESS, 0);

  // setup_timer1();
  // setup_timer2();

  lcd.begin(16, 2);
  lcd.print("Hello, void!");

  Timer1::instance().attach(timer1_callback);
  Timer1::instance().start(Timer1::SCALE_64, 25000 - 1);

  Timer2::instance().attach(timer2_callback);
  Timer2::instance().start(Timer2::SCALE_1024, 200);
}

void loop() {
  button1.update();
  if (button1.pressed()) {
    msec_counter = 0;
  }
  button1.flop();

  button2.update();
  if (button2.pressed()) {
    msec_counter += 1;
  } else
  if (button2.pressed_hard()) {
    msec_counter += 1;
  } else
  if (button2.pressed_repeat()) {
    msec_counter += 1;
  }
  button2.flop();

  button3.update();
  if (button3.pressed()) {
    msec_counter -= 1;
  }
  button3.flop();

  button4.update();
  if (button4.pressed()) {
    msec_counter = 65535;
  }
  button4.flop();

  if (msec_counter_tmp != msec_counter) { 
    msec_counter_tmp = msec_counter;
    char buff[16];
    memset(buff, 0, 16);
    sprintf(buff, "%05u", msec_counter);
    lcd.setCursor(0, 1);
    lcd.print(buff);
  }
}
