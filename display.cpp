#include "display.h"
#include "settings.h"

static const char *DAY_NAME[7] = {
    "Su",
    "Mo",
    "Tu",
    "We",
    "Th",
    "Fr",
    "Sa"
};

namespace DIGIT {
    char ZERO[8] = {
        0b11111,
        0b11111,
        0b11011,
        0b11011,
        0b11011,
        0b11011,
        0b11111,
        0b11111
    };

    char ONE[8] = {
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011
    };

    char TWO[8] = {
        0b11111,
        0b11111,
        0b00011,
        0b11111,
        0b11111,
        0b11000,
        0b11111,
        0b11111
    };

    char THREE[8] = {
        0b11111,
        0b11111,
        0b00011,
        0b01111,
        0b01111,
        0b00011,
        0b11111,
        0b11111
    };

    char FOUR[8] = {
        0b11000,
        0b11011,
        0b11011,
        0b11111,
        0b11111,
        0b00011,
        0b00011,
        0b00011
    };

    char FIVE[8] = {
        0b11111,
        0b11111,
        0b11000,
        0b11111,
        0b11111,
        0b00011,
        0b11111,
        0b11111
    };

    char SIX[8] = {
        0b11111,
        0b11111,
        0b11000,
        0b11111,
        0b11111,
        0b11011,
        0b11111,
        0b11111
    };

    char SEVEN[8] = {
        0b11111,
        0b11111,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011
    };

    char EIGHT[8] = {
        0b11111,
        0b11111,
        0b11011,
        0b11111,
        0b11111,
        0b11011,
        0b11111,
        0b11111
    };

    char NINE[8] = {
        0b11111,
        0b11111,
        0b11011,
        0b11111,
        0b11111,
        0b00011,
        0b11111,
        0b11111
    };

    char TEN[8] = {
        0b00000,
        0b00000,
        0b10111,
        0b10101,
        0b10101,
        0b10101,
        0b10111,
        0b00000
    };
};

Display::Display() 
    : _lcd(Pin::Display::RS, Pin::Display::ENABLE, 
            Pin::Display::D0, Pin::Display::D1, Pin::Display::D2, Pin::Display::D3) 
{
    
}

void Display::setup(uint8_t contrast_pin, uint8_t brightness_pin) {
    _contrast_pin = contrast_pin;
    _brightness_pin = brightness_pin;

    pinMode(_contrast_pin, OUTPUT);
    pinMode(_brightness_pin, OUTPUT);

    _load_digit(10, 0);

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

    print_text(1, 0, "                ");
}

void Display::_load_digit(uint8_t d, uint8_t p) {
    char *c = NULL;

    switch (d) {
        case 0:
            c = DIGIT::ZERO;
            break;
        case 1:
            c = DIGIT::ONE;
            break;
        case 2:
            c = DIGIT::TWO;
            break;
        case 3:
            c = DIGIT::THREE;
            break;
        case 4:
            c = DIGIT::FOUR;
            break;
        case 5:
            c = DIGIT::FIVE;
            break;
        case 6:
            c = DIGIT::SIX;
            break;
        case 7:
            c = DIGIT::SEVEN;
            break;
        case 8:
            c = DIGIT::EIGHT;
            break;
        case 9:
            c = DIGIT::NINE;
            break;
        case 10:
            c = DIGIT::TEN;
            break;
    }
    _lcd.createChar(p, c);
}

void Display::print_clock_time(const DateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, " %2u:%02u:%02u ", 
                  dt.hour,
                  dt.minute,
                  dt.second);
    print_text(0, 3, buff);
}

void Display::print_clock_date(const DateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u.%02u.%04u %s", 
                  dt.day,
                  dt.month,
                  dt.year,
                  DAY_NAME[dt.day_of_week]);
    print_text(1, 3, buff);
}

void Display::print_clock_datetime(const DateTime &dt) {
    print_clock_time(dt);
    print_clock_date(dt);
}

void Display::print_alarm_type1(const AlarmDateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u  %s", 
                  dt.hour,
                  dt.minute,
                  (dt.on ? "on " : "off"));
    print_text(0, 4, buff);

    memset(buff, ' ', sizeof(buff) - 1);
    print_text(1, 0, buff);
}

void Display::print_alarm_type2(const AlarmDateTime &dt) {
    print_alarm_type1(dt);
}

void Display::print_alarm_type3(const AlarmDateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u  %s", 
                  dt.hour,
                  dt.minute,
                  (dt.on ? "on " : "off"));
    print_text(0, 4, buff);

    memset(buff, ' ', 16);
    buff[10 + 1] = (dt.days[0] ? 'S' : 's');
    buff[ 4 + 1] = (dt.days[1] ? 'M' : 'm');
    buff[ 5 + 1] = (dt.days[2] ? 'T' : 't');
    buff[ 6 + 1] = (dt.days[3] ? 'W' : 'w');
    buff[ 7 + 1] = (dt.days[4] ? 'T' : 't');
    buff[ 8 + 1] = (dt.days[5] ? 'F' : 'f');
    buff[ 9 + 1] = (dt.days[6] ? 'S' : 's');
    print_text(1, 0, buff);
}

void Display::print_signal(bool on) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%s       ", (on ? "on " : "off"));
    print_text(0, 4, buff);

    memset(buff, ' ', sizeof(buff) - 1);
    print_text(1, 0, buff);
}

void Display::print_timer_type1(const TimerDateTime &dt) {
    char buff[16];

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u:%02u %s", 
                  dt.origin_hour,
                  dt.origin_minute,
                  dt.origin_second,
                  (dt.stoppped ? "off" : "on "));
    print_text(0, 4, buff);

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u:%02u  ", 
                  dt.hour,
                  dt.minute,
                  dt.second);
    print_text(1, 4, buff);
}

void Display::print_timer_type2(const TimerDateTime &dt) {
    print_timer_type1(dt);
}

void Display::print_timer_type3(const TimerDateTime &dt) {
    char buff[16];

    memset(buff, 0, 16);
    sprintf(buff, "%2u.%02u.%02u %s", 
                  dt.origin_day,
                  dt.origin_month,
                  dt.origin_year % 100,
                  (dt.stoppped ? "off" : "on "));
    print_text(0, 4, buff);

    memset(buff, 0, 16);

    if (dt.day < 1000) {
        sprintf(buff, "%3u %2u:%02u:%02u", 
                      dt.day,
                      dt.hour,
                      dt.minute,
                      dt.second);
        print_text(1, 0, buff);
    } else {
        uint8_t c = 0;
        _lcd.setCursor(0, 1);
        _lcd.write(c);
        sprintf(buff, "%2u %2u:%02u:%02u", 
                      dt.day % 100,
                      dt.hour,
                      dt.minute,
                      dt.second);
        print_text(2, 1, buff);
    }
}

void Display::print_stopwatch(const StopwatchTime &t) {
    char buff[17];

    memset(buff, 0, 17);
    sprintf(buff, "%02u:%02u:%02u.%01u   ", 
                  t.hour,
                  t.minute,
                  t.second,
                  t.ms / 100);
    print_text(0, 3, buff);

    memset(buff, 0, 17);

    if (!t.stamps[t.stamps_index_to_show].ready) {
        sprintf(buff, "   --:--:--.-  ");
    } else {
        sprintf(buff, "%2u %02u:%02u:%02u.%01u   ", 
                      t.stamps_index_to_show + 1,
                      t.stamps[t.stamps_index_to_show].hour,
                      t.stamps[t.stamps_index_to_show].minute,
                      t.stamps[t.stamps_index_to_show].second,
                      t.stamps[t.stamps_index_to_show].ms / 100);
    }
      
    print_text(1, 0, buff);
}

void Display::set_contrast(uint8_t contrast) {
    _contrast = contrast;
    analogWrite(_contrast_pin, _contrast);
}

void Display::set_brightness(uint8_t brightness) {
    _brightness = brightness;
    analogWrite(_brightness_pin, _brightness);
}

void Display::clear() {
  _lcd.clear();
}

bool Display::_blink() const {
    return ((millis() % 1000) < 250);
}

void Display::print_edit_clock(const DateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (_blink()) {
        char buff[2][17];
        memset(buff, 0, sizeof(buff));

        if (state == Clock::S_CLOCK_EDIT_SECONDS) {
            sprintf(buff[0], " %2u:%02u:  ", dt.hour, dt.minute);
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, DAY_NAME[dt.day_of_week]);
        } else 
        if (state == Clock::S_CLOCK_EDIT_MINUTES) {
            sprintf(buff[0], " %2u:  :%02u", dt.hour, dt.second);
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, DAY_NAME[dt.day_of_week]);
        } else 
        if (state == Clock::S_CLOCK_EDIT_HOURS) {
            sprintf(buff[0], "   :%02u:%02u", dt.minute, dt.second);
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, DAY_NAME[dt.day_of_week]);
        } else 
        if (state == Clock::S_CLOCK_EDIT_DAYS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "  .%02u.%04u %s", dt.month, dt.year, DAY_NAME[dt.day_of_week]);
        } else
        if (state == Clock::S_CLOCK_EDIT_MONTHS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "%2u.  .%04u %s", dt.day, dt.year, DAY_NAME[dt.day_of_week]);
        } else 
        if (state == Clock::S_CLOCK_EDIT_YEARS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "%2u.%02u.     %s", dt.day, dt.month, DAY_NAME[dt.day_of_week]);
        }

        print_text(0, 3, buff[0]);
        print_text(1, 3, buff[1]);
    } else {
        print_clock_datetime(dt);
    }
}

void Display::print_edit_alarm_type1(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (_blink()) {
        char buff[17];
        memset(buff, 0, sizeof(buff));

        if (state == Clock::S_ALARM_TYPE1_EDIT_MINUTES)
            sprintf(buff, "%2u:    %s", 
                          dt.hour,
                          (dt.on ? "on " : "off"));
        else
        if (state == Clock::S_ALARM_TYPE1_EDIT_HOURS)
            sprintf(buff, "  :%02u  %s", 
                          dt.minute,
                          (dt.on ? "on " : "off"));

        print_text(0, 4, buff);
    } else {
        print_alarm_type1(dt);
    }
}

void Display::print_edit_alarm_type2(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    print_edit_alarm_type1(dt, state);
}

void Display::print_edit_alarm_type3(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    char buff[17];
    memset(buff, 0, sizeof(buff));

    if (_blink()) {
        if (state == Clock::S_ALARM_TYPE2_EDIT_MINUTES)
            sprintf(buff, "%2u:    %s", 
                          dt.hour,
                          // dt.minute,
                          (dt.on ? "on" : "off"));
        else
        if (state == Clock::S_ALARM_TYPE2_EDIT_HOURS)
            sprintf(buff, "  :%02u  %s", 
                          // dt.hour,
                          dt.minute,
                          (dt.on ? "on " : "off"));
        else
            sprintf(buff, "%2u:%02u  %s", 
                      dt.hour,
                      dt.minute,
                      (dt.on ? "on " : "off"));
    } else {
        sprintf(buff, "%2u:%02u  %s", 
                      dt.hour,
                      dt.minute,
                      (dt.on ? "on " : "off"));
    }

    print_text(0, 4, buff);

    memset(buff, ' ', 16);

    if (state == Clock::S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK) {
        if (((millis() % 1000) < 250)) {
            buff[10 + 1] = (dt.day_pointer == 0 ? ' ' : (dt.days[0] ? 'S' : 's'));
            buff[ 4 + 1] = (dt.day_pointer == 1 ? ' ' : (dt.days[1] ? 'M' : 'm'));
            buff[ 5 + 1] = (dt.day_pointer == 2 ? ' ' : (dt.days[2] ? 'T' : 't'));
            buff[ 6 + 1] = (dt.day_pointer == 3 ? ' ' : (dt.days[3] ? 'W' : 'w'));
            buff[ 7 + 1] = (dt.day_pointer == 4 ? ' ' : (dt.days[4] ? 'T' : 't'));
            buff[ 8 + 1] = (dt.day_pointer == 5 ? ' ' : (dt.days[5] ? 'F' : 'f'));
            buff[ 9 + 1] = (dt.day_pointer == 6 ? ' ' : (dt.days[6] ? 'S' : 's'));
        } else {
            buff[10 + 1] = (dt.days[0] ? 'S' : 's');
            buff[ 4 + 1] = (dt.days[1] ? 'M' : 'm');
            buff[ 5 + 1] = (dt.days[2] ? 'T' : 't');
            buff[ 6 + 1] = (dt.days[3] ? 'W' : 'w');
            buff[ 7 + 1] = (dt.days[4] ? 'T' : 't');
            buff[ 8 + 1] = (dt.days[5] ? 'F' : 'f');
            buff[ 9 + 1] = (dt.days[6] ? 'S' : 's');
        }
    } else {
            buff[10 + 1] = (dt.days[0] ? 'S' : 's');
            buff[ 4 + 1] = (dt.days[1] ? 'M' : 'm');
            buff[ 5 + 1] = (dt.days[2] ? 'T' : 't');
            buff[ 6 + 1] = (dt.days[3] ? 'W' : 'w');
            buff[ 7 + 1] = (dt.days[4] ? 'T' : 't');
            buff[ 8 + 1] = (dt.days[5] ? 'F' : 'f');
            buff[ 9 + 1] = (dt.days[6] ? 'S' : 's');
    }
    print_text(1, 0, buff);
}

void Display::print_edit_timer_type1(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (_blink()) {
        char buff[17];
        memset(buff, 0, sizeof(buff));

        if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS)
            sprintf(buff, "%2u:%02u:   %s", 
                          dt.origin_hour,
                          dt.origin_minute,
                          //dt.origin_second,
                          (dt.stoppped ? "off" : "on "));
        else
        if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES)
            sprintf(buff, "%2u:  :%02u %s", 
                          dt.origin_hour,
                          // dt.origin_minute,
                          dt.origin_second,
                          (dt.stoppped ? "off" : "on "));
        else
        if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS)
            sprintf(buff, "  :%02u:%02u %s", 
                          // dt.origin_hour,
                          dt.origin_minute,
                          dt.origin_second,
                          (dt.stoppped ? "off" : "on "));

        print_text(0, 4, buff);
    } else {
        print_timer_type1(dt);
    }
}

void Display::print_edit_timer_type2(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    print_edit_timer_type1(dt, state);
}

void Display::print_edit_timer_type3(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (_blink()) {
        char buff[17];
        memset(buff, 0, sizeof(buff));

        if (state == Clock::S_TIMER_TYPE2_EDIT_DAYS)
            sprintf(buff, "  .%02u.%02u %s", 
                          // dt.origin_day,
                          dt.origin_month,
                          dt.origin_year % 100,
                          (dt.on ? "on " : "off"));
        else
        if (state == Clock::S_TIMER_TYPE2_EDIT_MONTHS)
            sprintf(buff, "%2u.  .%02u %s", 
                          dt.origin_day,
                          // dt.origin_month,
                          dt.origin_year % 100,
                          (dt.on ? "on " : "off"));
        else
        if (state == Clock::S_TIMER_TYPE2_EDIT_YEARS)
            sprintf(buff, "%2u.%02u.   %s", 
                      dt.origin_day,
                      dt.origin_month,
                      // dt.origin_year,
                      (dt.on ? "on " : "off"));

        print_text(0, 4, buff);
    } else {
        print_timer_type3(dt);
    }
}

uint8_t Display::contrast() const {
    return _contrast;
}

uint8_t Display::brightness() const {
    return _brightness;
}
