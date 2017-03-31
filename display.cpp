#include "display.h"

// For LiquidCrystal
static const uint8_t _RS_ = 8; 
static const uint8_t _ENABLE_ = 9;
static const uint8_t _D0_ = 4;
static const uint8_t _D1_ = 5; 
static const uint8_t _D2_ = 6; 
static const uint8_t _D3_ = 7;

Display::Display() 
    : _lcd(_RS_, _ENABLE_, _D0_, _D1_, _D2_, _D3_) {
    
}

void Display::setup(uint8_t contrast_pin, uint8_t brightness_pin) {
    _contrast_pin = contrast_pin;
    _brightness_pin = brightness_pin;

    pinMode(_contrast_pin, OUTPUT);
    pinMode(_brightness_pin, OUTPUT);

    _lcd.begin(16, 2);
}

void Display::print_text(uint8_t row, uint8_t column, char *s) {
    _lcd.setCursor(column, row);
    _lcd.print(s);
}

void Display::print_clock_state(const Clock &clock) {
    switch (clock.state()) {
        case Clock::S_CLOCK1:
            print_text(0, 0, "CL1");
            break;
        case Clock::S_CLOCK2:
            print_text(0, 0, "CL2");
            break;
        case Clock::S_ALARM1:
            print_text(0, 0, "AL1");
            break;
        case Clock::S_ALARM2:
            print_text(0, 0, "AL2");
            break;
        case Clock::S_ALARM3:
            print_text(0, 0, "AL3");
            break;
        case Clock::S_SIGNAL:
            print_text(0, 0, "SG ");
            break;
        case Clock::S_TIMER1:
            print_text(0, 0, "T1 ");
            break;
        case Clock::S_TIMER2:
            print_text(0, 0, "T2 ");
            break;
        case Clock::S_TIMER3:
            print_text(0, 0, "T3 ");
            break;
        case Clock::S_STOPWATCH:
            print_text(0, 0, "SW ");
            break;
    }
}

void Display::print_time(const DateTime &dt) {
    char buff[16];

    memset(buff, 0, 16);
    sprintf(buff, "%2u:%02u:%02u", 
                  dt.hour,
                  dt.minute,
                  dt.second);
    print_text(0, 4, buff);
}
void Display::print_date(const DateTime &dt) {
    char buff[16];
    
    memset(buff, 0, 16);
    sprintf(buff, "%2u.%02u.%04u", 
                  dt.day,
                  dt.month,
                  dt.year);
    print_text(1, 3, buff);
}

void Display::print_datetime(const DateTime &dt) {
    print_time(dt);
    print_date(dt);
}

void Display::set_contrast(uint8_t contrast) {
    _contrast = contrast;
    analogWrite(_contrast_pin, _contrast);
}

void Display::set_brightness(uint8_t brightness) {
    _brightness = brightness;
    analogWrite(_brightness_pin, _brightness);
}

uint8_t Display::contrast() const {
    return _contrast;
}

uint8_t Display::brightness() const {
    return _brightness;
}
