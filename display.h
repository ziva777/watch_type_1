#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "clock.h"

class Display {
    public:
        Display();

        void setup(uint8_t contrast_pin, uint8_t brightness_pin);
        void set_contrast(uint8_t contrast);
        void set_brightness(uint8_t brightness);

        void print_text(uint8_t row, uint8_t column, char *s);
        void print_clock_state(const Clock &clock);

        uint8_t contrast() const;
        uint8_t brightness() const;

    private:
        LiquidCrystal _lcd;
        uint8_t _contrast_pin;
        uint8_t _brightness_pin;
        uint8_t _contrast;
        uint8_t _brightness;
};

#endif // _DISPLAY_H_
