#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "clock.h"
#include "datetime.h"

class Display {
    public:
        Display();

        void setup(uint8_t contrast_pin, uint8_t brightness_pin);
        void set_contrast(uint8_t contrast);
        void set_brightness(uint8_t brightness);
        void clear();

        void print_text(uint8_t row, uint8_t column, char *s);
        void print_clock_state(const Clock &clock);
        void print_time(const DateTime &dt);
        void print_date(const DateTime &dt);
        void print_datetime(const DateTime &dt);
        void print_alarm1(const AlarmDateTime &dt);
        void print_alarm2(const AlarmDateTime &dt);
        void print_alarm3(const AlarmDateTime &dt);
        void print_signal(bool on);
        void print_timer1(const TimerDateTime &dt);
        void print_timer2(const TimerDateTime &dt);
        void print_timer3(const TimerDateTime &dt);
        void print_stopwatch(const StopwatchTime &t);

        void print_edit_clock(const DateTime &dt, Clock::CLOCK_SUBSTATES state);
        void print_edit_alarm_type1(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void print_edit_alarm_type2(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void print_edit_alarm_type3(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void print_edit_timer_type1(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void print_edit_timer_type2(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void print_edit_timer_type3(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state);

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
